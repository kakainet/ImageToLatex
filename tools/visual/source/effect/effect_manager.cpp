#include "effect_manager.hpp"

namespace itl
{
    EffectManager::EffectManager()
    {
        Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                    Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

        this->load_functions();
        this->generate_all_effect_packs();

        Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                    Logger::STREAM::CONSOLE, Logger::TYPE::INFO);
    }



    void EffectManager::load_functions()
    {
        /*this->functions[FUNCTION_T::SCALE] = std::vector<std::function<void(cv::Mat&)>>();
        this->functions[FUNCTION_T::SCALE].emplace_back([](cv::Mat& sprite)
                                                        {
                                                            auto val = Math::random_float(
                                                                    constants::effect::scale_incr_bounds.x,
                                                                    constants::effect::scale_incr_bounds.y,
                                                                    constants::effect::accuracy);
                                                            sprite.({val, val});
                                                        });

        this->functions[FUNCTION_T::SCALE].emplace_back([](cv::Mat& sprite)
                                                        {
                                                            auto rand = [](){return Math::random_float(
                                                                    constants::effect::scale_incr_bounds.x,
                                                                    constants::effect::scale_incr_bounds.y,
                                                                    constants::effect::accuracy);};
                                                            sprite.setScale({rand(), rand()});
                                                        });

        this->functions[FUNCTION_T::SCALE].emplace_back([](cv::Mat& sprite)
                                                        {
                                                            auto val = Math::random_float(
                                                                    constants::effect::scale_decr_bounds.x,
                                                                    constants::effect::scale_decr_bounds.y,
                                                                    constants::effect::accuracy);
                                                            sprite.setScale({val, val});
                                                        });

        this->functions[FUNCTION_T::SCALE].emplace_back([](cv::Mat& sprite)
                                                        {
                                                            auto rand = [](){return Math::random_float(
                                                                    constants::effect::scale_decr_bounds.x,
                                                                    constants::effect::scale_decr_bounds.y,
                                                                    constants::effect::accuracy);};
                                                            sprite.setScale({rand(), rand()});
                                                        });*/

        this->functions[FUNCTION_T::ROTATE] = std::vector<std::function<void(cv::Mat&)>>();
        this->functions[FUNCTION_T::ROTATE].emplace_back([this](cv::Mat& sprite)
                                                         {
                                                             auto val = Math::random_float(
                                                                     0,
                                                                     constants::effect::max_degree,
                                                                     constants::effect::accuracy);

                                                             this->rotate(sprite, val);

                                                         });

        this->functions[FUNCTION_T::ROTATE].emplace_back([this](cv::Mat& sprite)
                                                         {
                                                             auto val = Math::random_float(
                                                                     -constants::effect::max_degree,
                                                                     0,
                                                                     constants::effect::accuracy);

                                                             this->rotate(sprite, val);
                                                         });
    }


    void EffectManager::generate_all_effect_packs()
    {
        const int n = static_cast<int>(functions.size());

        int* indices = new int[n];

        for (int i = 0; i < n; i++)
        {
            indices[i] = 0;
        }

        while (true)
        {
            packIndexes.emplace_back();

            for (int i = 0; i < n; i++)
            {
                packIndexes.back().emplace_back(indices[i]);
            }

            int next = n - 1;
            while (next >= 0 && (indices[next] + 1 >= this->functions.at(static_cast<FUNCTION_T>(next)).size()))
            {
                next--;
            }

            if (next < 0)
            {
                return;
            }

            indices[next]++;

            for (int i = next + 1; i < n; i++)
            {
                indices[i] = 0;
            }
        }
    }

    std::vector<std::shared_ptr<cv::Mat>> itl::EffectManager::generateSprites(const cv::Mat &sprite)
    {
        auto result = std::vector<std::shared_ptr<cv::Mat>>();

        for(auto& indexPack: this->packIndexes)
        {
            cv::Mat new_sprite = sprite;

            for(size_t i = 0; i < indexPack.size(); i++)
            {
                auto fun = static_cast<FUNCTION_T>(i);
                this->functions[fun][indexPack[i]](new_sprite);
            }

            result.push_back(std::make_shared<cv::Mat>(new_sprite));
        }

        return result;
    }

    void EffectManager::rotate(cv::Mat &sprite, float angle)
    {
        cv::Point2f center((sprite.cols-1)/2.0, (sprite.rows-1)/2.0);
        cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
        cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), sprite.size(), angle).boundingRect2f();
        rot.at<double>(0,2) += bbox.width/2.0 - sprite.cols/2.0;
        rot.at<double>(1,2) += bbox.height/2.0 - sprite.rows/2.0;

        cv::Mat dst;
        cv::warpAffine(sprite, dst, rot, bbox.size());
        sprite = dst;
    }

    void EffectManager::move(cv::Mat &img, float pixels_dx, float pixels_dy)
    {
        cv::Mat trans_mat = (cv::Mat_<double>(2,3) << 1, 0, pixels_dx, 0, 1, pixels_dy);
        cv::warpAffine(img,img,trans_mat,img.size());
    }
}