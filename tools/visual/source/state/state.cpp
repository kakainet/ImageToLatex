#include "state.hpp"

namespace itl
{
    State::State(const std::shared_ptr<Logger>& log,
                 const std::shared_ptr<itl::FlagManager>& flag_manager)
        : flag_manager(flag_manager),
          logger(log),
          hardware_concurrency(std::thread::hardware_concurrency())
    {
        this->logger->log(std::string(cst::info::init_module_msg_start) +
                              std::string(typeid(this).name()),
                          Logger::stream_t::console, Logger::type_t::info);
        std::stringstream thread_info;
        thread_info << cst::thread::number_thread_info << " "
                    << std::to_string(this->hardware_concurrency);

        this->logger->log(thread_info.str(), Logger::stream_t::console,
                          Logger::type_t::info);

        this->thread_pool =
            std::make_unique<ThreadPool>(this->hardware_concurrency);
        this->transform = std::make_unique<Transform>(log);

        this->logger->log(std::string(cst::info::init_module_msg_end) +
                              std::string(typeid(this).name()),
                          Logger::stream_t::console, Logger::type_t::info);
    }

    int State::run(const std::string& path_to_data,
                   const std::string& extension)
    {
        // Effect manager depends on data so it is not created in ctor but there
        this->effect_manager =
            std::make_unique<EffectManager>(this->logger, path_to_data);
        return this->generate_images(path_to_data + cst::file::pic, extension)
                   ? cst::system::pass_code
                   : cst::system::error_code;
    }

    bool State::generate_images(const std::string& dir,
                                const std::string& extension)
    {
        auto glob_to_stdstring = [=](const std::string& where_glob,
                                     std::vector<std::string>& output_paths) {
            std::vector<cv::String> fn;
            glob(where_glob, fn, false);
            output_paths.resize(fn.size());

            std::transform(fn.begin(), fn.end(), output_paths.begin(),
                           [](const cv::String& str) {
                               return str.operator std::string();
                           });
        };
        std::vector<std::string> paths_pic;
        std::vector<std::string> paths_background;
        glob_to_stdstring(dir + cst::file::input + '*' + extension, paths_pic);
        glob_to_stdstring(dir + "../" + cst::file::texture + '*' + extension,
                          paths_background);

        std::string output = dir + cst::file::output;

        std::vector<std::future<bool>> results;
        for(size_t i = 0; i < paths_background.size(); i++)
        {
            for(auto& var : paths_pic)
            {
                std::string cp_output{output};
                std::string cp_ext{extension};
                results.emplace_back(this->thread_pool->enqueue(
                    &State::process_line, this, var, cp_output,
                    paths_background[i], static_cast<int>(i), cp_ext));
            }
        }

        return std::accumulate(
            results.begin(), results.end(), true,
            [=](bool acc, auto&& res) { return acc && res.get(); });
    }

    bool State::process_line(const std::string& path_to_raw,
                             const std::string& dir_to_save,
                             const std::string& path_to_background,
                             int background_idx,
                             const std::string& extension) noexcept
    {
        cv::Mat base(cv::imread(path_to_raw, cv::IMREAD_UNCHANGED));

        this->mtx.lock();
        cv::Mat background(
            cv::imread(path_to_background, cv::IMREAD_UNCHANGED));
        this->mtx.unlock();

        if(!base.data || !background.data ||
           background.rows != cst::window::dim ||
           background.cols != cst::window::dim)
        {
            std::scoped_lock<std::mutex> lck(this->mtx);
            std::stringstream ss;
            ss << cst::texture::failed_load_texture
               << "\n\tBase texture path: " << path_to_raw
               << "\n\tBackground texture path: " << path_to_background;
            this->logger->log(ss.str(), Logger::stream_t::console,
                              Logger::type_t::error);

            return false;
        }

        int itr = 0;

        std::string file_name;
        std::vector<std::shared_ptr<cv::Mat>> sprites;

        {
            file_name = (path_to_raw.substr(path_to_raw.find_last_of('/') + 1));
            file_name = file_name.substr(0, file_name.find_last_of('.'));
            sprites = this->effect_manager->generateSprites(base);
        }

        for(auto& spr : sprites)
        {
            std::stringstream path_to_save;
            path_to_save << dir_to_save << "/" << file_name << "_" <<
                background_idx << "_" << std::to_string(itr++) << extension;

            // apply position effect - it is separated due to
            // performance. Using relative position instead
            // of enlarging sprite is significantly faster
            cv::Mat dst(background.rows, background.cols, CV_8UC4);

            int dx = background.cols - spr->cols;
            int dy = background.rows - spr->rows;

            this->transform->merge_images(&dst, &background, 0, 0);
            this->transform->merge_images(&dst, &*spr,
                                          dx < 0 ? 0 : Math::random(0, dx),
                                          dy < 0 ? 0 : Math::random(0, dy));
            cv::imwrite(path_to_save.str(), dst);
        }

        return true;
    }
}  // namespace itl