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
        this->functions[FUNCTION_T::ROTATE].emplace_back([](cv::Mat& sprite)
                                                         {
                                                             auto val = Math::random_float(
                                                                     0,
                                                                     constants::effect::max_degree,
                                                                     constants::effect::accuracy);

                                                                 cv::Mat r = getRotationMatrix2D(
                                                                         cv::Point2f(sprite.cols/2.f,
                                                                                 sprite.rows/2.f),
                                                                                 val,
                                                                                 1.0);

                                                                 cv::warpAffine(sprite,
                                                                         sprite,
                                                                         r,
                                                                         cv::Size(sprite.cols, sprite.rows));

                                                         });

        this->functions[FUNCTION_T::ROTATE].emplace_back([](cv::Mat& sprite)
                                                         {
                                                             auto val = Math::random_float(
                                                                     -constants::effect::max_degree,
                                                                     0,
                                                                     constants::effect::accuracy);

                                                             cv::Mat r = getRotationMatrix2D(
                                                                     cv::Point2f(sprite.cols/2.f,
                                                                                 sprite.rows/2.f),
                                                                     val,
                                                                     1.0);

                                                             cv::warpAffine(sprite,
                                                                            sprite,
                                                                            r,
                                                                            cv::Size(sprite.cols, sprite.rows));
                                                         });

        this->functions[FUNCTION_T::POSITION].emplace_back([](cv::Mat& sprite)
                                                           {
                                                               auto val = cv::Vec2f(
                                                                       Math::random(0, constants::window::size(0)),
                                                                       Math::random(0, constants::window::size(1)));
                                                               //cv::Mat imgTranslated(sprite.size(),sprite.type(), cv::Scalar::all(0));
                                                               sprite(cv::Rect(val(0), val(1),sprite.cols-val(0),sprite.rows-val(1)))
                                                               .copyTo(sprite(cv::Rect(0,0,sprite.cols-val(0),sprite.rows-val(1))));
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

    std::vector<std::shared_ptr<cv::Mat>> itl::EffectManager::generateSprites(cv::Mat &sprite)
    {
        auto result = std::vector<std::shared_ptr<cv::Mat>>();

        for(auto& indexPack: this->packIndexes)
        {
            auto new_sprite = std::make_shared<cv::Mat>(sprite);

            for(int i = 0; i < indexPack.size(); i++)
            {
                auto fun = static_cast<FUNCTION_T>(i);
                this->functions[fun][indexPack[i]](*new_sprite);
            }

            result.push_back(new_sprite);
        }

        return result;
    }
}