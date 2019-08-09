#include "state.hpp"

namespace itl
{
    State::State(const std::string& title)
        :hardware_concurrency(1)
    {
        itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

        this->effect_manager = std::make_unique<EffectManager>();
        this->thread_pool = std::make_unique<ThreadPool>(this->hardware_concurrency);

        itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

        this->assigned_threads_to_data = 0;
    }

    int State::run(const std::string& path_to_pictures, const std::string& extension, const std::string& path_to_data)
    {
        return
                this->load_textures(path_to_data)
                && this->generate_images(path_to_pictures, extension)
                ? constants::system::pass_code : constants::system::error_code;
    }

    bool State::load_textures(const std::string& path_to_data) noexcept
    {
        return true;
    }

    bool State::generate_images(const std::string& dir, const std::string& extension)
    {
        std::vector<cv::String> fn;
        std::vector<std::string> paths;
        glob(dir + "/input/*" + extension, fn, false);
        paths.resize(fn.size());

        auto convert_cvstr_to_str = [](cv::String str)
                {
                    return str.operator std::string();
                };

        std::transform(fn.begin(), fn.end(), paths.begin(), convert_cvstr_to_str);

        std::string output = dir + "/output";

        std::vector<std::future<bool>> results;
        for(int i = 0; i < 1; i++)
        {
            for(auto& var: paths)
            {
                std::string cp_output{output};
                int cp_itr{i};
                std::string cp_ext{extension};
                results.emplace_back(this->thread_pool->enqueue(&State::process_line, this, var, cp_output, cp_itr, cp_ext));
            }
        }

        return std::accumulate(results.begin(), results.end(), true,
                [=](bool acc, auto&& res) { return acc && res.get(); });
    }

    bool State::process_line(const std::string& path_to_raw, const std::string& dir_to_save,
                      int background_number, const std::string& extension) noexcept
    {
        cv::Mat base(cv::imread(path_to_raw, CV_LOAD_IMAGE_COLOR));
        cv::Mat background(cv::imread("data/textures/white.png", CV_LOAD_IMAGE_COLOR));
        if(!base.data || !background.data)
        {
            std::scoped_lock<std::mutex> lck(this->mtx);
            Logger::Log(constants::texture::failed_load_texture, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
            return false;
        }

        int itr = 0;

        std::string file_name;
        std::vector<std::shared_ptr<cv::Mat>> sprites;

        {
            std::scoped_lock<std::mutex> lck(this->mtx);
            file_name = (path_to_raw.substr(path_to_raw.find_last_of('/')+1));
            file_name = file_name.substr(0, file_name.find_last_of('.'));
            sprites = this->effect_manager->generateSprites(base);
        }

        for(auto& spr : sprites)
        {
            std::stringstream path_to_save;
            std::scoped_lock<std::mutex> lck(this->mtx);

            path_to_save << dir_to_save
                         << "/"
                         << background_number
                         << "_" << file_name
                         << "_"
                         << std::to_string(itr++)
                         << extension;

            cv::Mat dst;
            cv::add(background, *spr, dst);
            cv::imwrite(path_to_save.str(), dst);
        }

        return true;
    }
}