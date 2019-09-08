#pragma once


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
        Transform();

        void merge_images(cv::Mat* background, cv::Mat* upcoming, int x, int y);

        void rotate(cv::Mat& sprite, float angle);

        void move(cv::Mat &img, float pixels_dx, float pixels_dy);

    };
}

