#pragma once

#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../logger/logger.hpp"

namespace itl
{
    class Transform
    {
    public:
        explicit Transform(const std::shared_ptr<Logger>& log);

        void merge_images(cv::Mat* background, cv::Mat* upcoming, int x, int y);

        void rotate(cv::Mat& sprite, float angle);

        void move(cv::Mat &img, float pixels_dx, float pixels_dy);

        void scale(cv::Mat& sprite, float factor_x, float factor_y);

    private:
        std::shared_ptr<Logger> logger;

    };
}

