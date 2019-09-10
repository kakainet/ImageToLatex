#pragma once

#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <cstring>

#include "../logger/logger.hpp"
#include "../config/config.hpp"
#include "../math/math.hpp"
#include "../transform/transform.hpp"

namespace itl
{
    class EffectManager
    {
    public:
        explicit EffectManager(const std::shared_ptr<Logger>& log);
        std::vector<std::shared_ptr<cv::Mat>> generateSprites(const cv::Mat& sprite);

    private:

        enum class FUNCTION_T
        {
            ROTATE,
            SCALE
        };

        std::map<FUNCTION_T, std::vector<std::function<void(cv::Mat&)>>> functions;

        /**
         * @brief packIndexes[i][j] is index of j'th effect from i'th pack.
         */
        std::vector<std::vector<int>> pack_indexes;

        void load_functions();

        void generate_all_effect_packs();

        std::unique_ptr<Transform> transform;

        std::shared_ptr<Logger> logger;

    };
}