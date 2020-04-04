#include "state.hpp"

namespace itl
{
    State::State(const std::shared_ptr<itl::FlagManager>& flag_manager)
        : flag_manager(flag_manager),
          hardware_concurrency(std::thread::hardware_concurrency())
    {
        std::stringstream thread_info;
        thread_info << cst::thread::number_thread_info << " "
                    << std::to_string(this->hardware_concurrency);

        Logger::log(thread_info.str(), Logger::stream_t::console, Logger::type_t::info);
        this->thread_pool =
            std::make_unique<ThreadPool>(this->hardware_concurrency);
        this->transform = std::make_unique<Transform>();

        this->background_manager = std::make_unique<ImageManager>();
    }

    int State::run(const std::string& path_to_data,
                   const std::string& extension)
    {
        // Effect manager depends on data so it is not created in ctor but there
        this->effect_manager = std::make_unique<EffectManager>(path_to_data);
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
            cv::glob(where_glob, fn, false);
            output_paths.resize(fn.size());

            std::transform(
                fn.begin(), fn.end(), output_paths.begin(),
                [](const cv::String& str) { return std::string(str); });
        };
        std::vector<std::string> paths_pic;
        std::vector<std::string> paths_background;
        glob_to_stdstring(dir + cst::file::input + '*' + extension, paths_pic);
        glob_to_stdstring(dir + "../" + cst::file::texture + '*' + extension,
                          paths_background);

        std::string output = dir + cst::file::output;

        std::vector<std::future<bool>> results;
        if(!background_manager->append(paths_background))
        {
            std::stringstream ss;
            ss << cst::texture::failed_load_texture
               << " One or more backgrounds were failed during loading.";
            Logger::log(ss.str());
            return false;
        }

        for(size_t i = 0; i < paths_background.size(); i++)
        {
            for(auto& var : paths_pic)
            {
                std::string cp_output{output};
                std::string cp_ext{extension};
                cv::Mat bck = (*this->background_manager)[i];
                results.emplace_back(this->thread_pool->enqueue(
                    &State::process_line, this, var, cp_output,
                    (*this->background_manager)[i].clone(), static_cast<int>(i),
                    cp_ext));
            }
        }

        return std::accumulate(
            results.begin(), results.end(), true,
            [=](bool acc, auto&& res) { return acc && res.get(); });
    }

    bool State::process_line(const std::string& path_to_raw,
                             const std::string& dir_to_save, cv::Mat background,
                             int background_idx,
                             const std::string& extension) noexcept
    {
        cv::Mat base(cv::imread(path_to_raw, cv::IMREAD_UNCHANGED));

        if(!base.data)
        {
            std::scoped_lock<std::mutex> lck(this->mtx);
            std::stringstream ss;
            ss << cst::texture::failed_load_texture
               << "\n\tBase texture path: " << path_to_raw;
            Logger::log(ss.str());
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
            path_to_save << dir_to_save << "/" << file_name << "_"
                         << background_idx << "_" << std::to_string(itr++)
                         << extension;

            // apply position effect - it is separated due to
            // performance. Using relative position instead
            // of enlarging sprite is significantly faster
            cv::Mat dst(background.rows, background.cols, CV_8UC4);

            int dx = background.cols - spr->cols;
            int dy = background.rows - spr->rows;

            this->transform->merge_images(&dst, &background, 0, 0);
            this->transform->merge_images(&dst, &*spr, dx < 0 ? 0 : dx / 2,
                                          dy < 0 ? 0 : dy / 2);
            cv::imwrite(path_to_save.str(), dst);
        }

        return true;
    }
}  // namespace itl