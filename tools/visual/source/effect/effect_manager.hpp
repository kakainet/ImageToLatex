#pragma once

#include <memory>
#include <vector>
#include <map>
#include <functional>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../logger/logger.hpp"
#include "../config/config.hpp"
#include "../math/math.hpp"

#include <cstring>

namespace itl
{
    class EffectManager
    {
    public:
        EffectManager();
        std::vector<std::shared_ptr<cv::Mat>> generateSprites(cv::Mat& sprite);

    private:

        enum class FUNCTION_T
        {
            SCALE = 0,
            ROTATE,
            POSITION
        };

        std::map<FUNCTION_T, std::vector<std::function<void(cv::Mat&)>>> functions;

        /**
         * @brief packIndexes[i][j] is index of j'th effect from i'th pack.
         */
        std::vector<std::vector<int>> packIndexes;

        void load_functions();

        void generate_all_effect_packs();

    };
}