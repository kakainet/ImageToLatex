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

    void merge_images(cv::Mat* background, cv::Mat* upcoming, int x, int y)
    {
        auto handle_cv_8uc4 = [=](int i, int j)
                {
                    if(upcoming->at<cv::Vec4b>(j, i)[3] > constants::effect::alpha_trash_hold)
                    {
                        background->at<cv::Vec4b>(y+j, x+i) = upcoming->at<cv::Vec4b>(j, i);
                    }
                };

        auto handle_cv_8uc3 = [=](int i, int j)
        {
            background->at<cv::Vec4b>(y+j, x+i)[0] = upcoming->at<cv::Vec3b>(j, i)[0];
            background->at<cv::Vec4b>(y+j, x+i)[1] = upcoming->at<cv::Vec3b>(j, i)[1];
            background->at<cv::Vec4b>(y+j, x+i)[2] = upcoming->at<cv::Vec3b>(j, i)[2];
            background->at<cv::Vec4b>(y+j, x+i)[3] = std::numeric_limits<uint8_t>::max() - 1;
        };

        for(int i = 0; i < upcoming->cols; i++)
        {
            for(int j = 0; j < upcoming->rows; j++)
            {
                if(j + y >= background->rows)
                {
                    break;
                }

                if(x + i >= background->cols)
                {
                    return;
                }

                switch(upcoming->channels())
                {
                    case constants::effect::rgb_channel_idx:
                    {
                        handle_cv_8uc3(i, j);
                        break;
                    }

                    case constants::effect::rgba_channel_idx:
                    {
                        handle_cv_8uc4(i, j);
                        break;
                    }

                    default:
                    {
                        itl::Logger::Log(constants::effect::failed_merging,
                                         Logger::STREAM::BOTH, Logger::TYPE::ERROR);
                        return;
                    }
                }

            }
        }
    }

    bool State::process_line(const std::string& path_to_raw, const std::string& dir_to_save,
                      int background_number, const std::string& extension) noexcept
    {
        cv::Mat base(cv::imread(path_to_raw, cv::IMREAD_UNCHANGED));
        cv::Mat background(cv::imread("data/textures/blue.png", cv::IMREAD_UNCHANGED));

        if(!base.data || !background.data)
        {
            std::scoped_lock<std::mutex> lck(this->mtx);
            std::stringstream ss;
            ss << constants::texture::failed_load_texture
               << "\n\tBase texture path: "
               << path_to_raw
               << "\n\tBackground texture path: "
               << "data/textures/white.png";
            Logger::Log(ss.str(), Logger::STREAM::BOTH, Logger::TYPE::ERROR);
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

            //put is exactly position effect - it is separated due to performance
            //using relative position instead of enlarging sprite is significantly faster
            cv::Mat dst(background.rows, background.cols, CV_8UC4);

            int dx = background.cols - spr->cols;
            int dy = background.rows - spr->rows;

            merge_images(&dst, &background, 0, 0);
            merge_images(&dst, &*spr, dx < 0 ? 0 : Math::random(0, dx), dy < 0 ? 0 : Math::random(0, dy));
            cv::imwrite(path_to_save.str(), dst);

        }

        return true;
    }
}