#pragma once

#include <vector>
#include <numeric>

#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../logger/logger.hpp"

namespace itl
{
    class ImageManager
    {
    public:
        ImageManager();
        bool append(const std::string& path);
        bool append(const std::vector<std::string>& paths);
        cv::Mat& operator[](int idx);
    private:
        std::vector<cv::Mat> imgs;
        Logger logger; ///< ptr to logger
    };
}