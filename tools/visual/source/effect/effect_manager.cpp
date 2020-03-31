#include "effect_manager.hpp"

namespace itl
{
    EffectManager::EffectManager(const std::shared_ptr<Logger>& log,
                                 const std::string& path_to_data)
        : logger(log)
    {
        this->perlin_noise =
            std::make_unique<PerlinNoise>(logger, path_to_data);
        this->load_functions();
        this->generate_all_effect_packs();
    }

    void EffectManager::load_functions()
    {
        this->functions[function_t::rotate] =
            std::vector<std::function<void(cv::Mat&)>>();
        this->functions[function_t::rotate].emplace_back(
            [this](cv::Mat& sprite) {
                auto val = Math::random_float(0, cst::effect::max_degree,
                                              cst::effect::accuracy);

                this->transform->rotate(sprite, val);
            });

        this->functions[function_t::rotate].emplace_back(
            [this](cv::Mat& sprite) {
                auto val = Math::random_float(-cst::effect::max_degree, 0,
                                              cst::effect::accuracy);

                this->transform->rotate(sprite, val);
            });

        this->functions[function_t::scale] =
            std::vector<std::function<void(cv::Mat&)>>();
        this->functions[function_t::scale].emplace_back([this](
                                                            cv::Mat& sprite) {
            auto val = Math::random_float(cst::effect::scale_incr_bounds(0),
                                          cst::effect::scale_incr_bounds(1),
                                          cst::effect::accuracy);
            this->transform->scale_clamp(sprite, val, val, 0, cst::window::dim);
        });

        this->functions[function_t::scale].emplace_back([this](
                                                            cv::Mat& sprite) {
            auto val = Math::random_float(cst::effect::scale_decr_bounds(0),
                                          cst::effect::scale_decr_bounds(1),
                                          cst::effect::accuracy);
            this->transform->scale_clamp(sprite, val, val, 0, cst::window::dim);
        });

        this->functions[function_t::scale].emplace_back(
            [this](cv::Mat& sprite) {
                auto rand = []() {
                    return Math::random_float(cst::effect::scale_incr_bounds(0),
                                              cst::effect::scale_incr_bounds(1),
                                              cst::effect::accuracy);
                };
                this->transform->scale_clamp(sprite, rand(), rand(), 0,
                                             cst::window::dim);
            });

        this->functions[function_t::scale].emplace_back(
            [this](cv::Mat& sprite) {
                auto rand = []() {
                    return Math::random_float(cst::effect::scale_decr_bounds(0),
                                              cst::effect::scale_decr_bounds(1),
                                              cst::effect::accuracy);
                };
                this->transform->scale_clamp(sprite, rand(), rand(), 0,
                                             cst::window::dim);
            });
    }

    void EffectManager::generate_all_effect_packs()
    {
        const int n = static_cast<int>(functions.size());

        int* indices = new int[n];

        for(int i = 0; i < n; i++)
        {
            indices[i] = 0;
        }

        while(true)
        {
            pack_indexes.emplace_back();

            for(int i = 0; i < n; i++)
            {
                pack_indexes.back().emplace_back(indices[i]);
            }

            int next = n - 1;
            while(
                next >= 0 &&
                (indices[next] + 1 >=
                 static_cast<int>(
                     this->functions.at(static_cast<function_t>(next)).size())))
            {
                next--;
            }

            if(next < 0)
            {
                return;
            }

            indices[next]++;

            for(int i = next + 1; i < n; i++)
            {
                indices[i] = 0;
            }
        }
    }

    std::vector<std::shared_ptr<cv::Mat>> EffectManager::generateSprites(
        const cv::Mat& sprite)
    {
        auto result = std::vector<std::shared_ptr<cv::Mat>>();

        for(auto& indexPack : this->pack_indexes)
        {
            cv::Mat new_sprite = sprite;

            for(size_t i = 0; i < indexPack.size(); i++)
            {
                auto fun = static_cast<function_t>(i);
                this->functions[fun][indexPack[i]](new_sprite);
            }

            result.push_back(std::make_shared<cv::Mat>(new_sprite));
        }

        return result;
    }
}  // namespace itl