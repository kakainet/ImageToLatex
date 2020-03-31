#ifndef ITL_TRANSFORM_HPP
#define ITL_TRANSFORM_HPP

#include <memory>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../logger/logger.hpp"

namespace itl
{
    /**
     * class for applying basic transformations of image
     */
    class Transform
    {
       public:
        /**
         * @brief explicit ctor
         * @param log - ptr to logger
         */
        explicit Transform();

        /**
         * @brief merges two images and save result in background
         * @param background - background image
         * @param upcoming - sprite
         * @param x - position x of left upper corner where upcoming will be put
         * @param y - position y of left upper corner where upcoming will be put
         */
        void merge_images(cv::Mat* background, cv::Mat* upcoming, int x, int y);

        /**
         * @brief rotate image by center origin
         * @param sprite - sprite to rotate
         * @param angle - angle of rotation
         */
        void rotate(cv::Mat& sprite, float angle);

        /**
         * @brief moves sprite
         * @param sprite - sprite to be moved
         * @param pixels_dx - delta of position in OX
         * @param pixels_dy - delta of position in OY
         */
        void move(cv::Mat& sprite, float pixels_dx, float pixels_dy);

        /**
         * @brief scales sprite by given factors
         * @param sprite - image to be scaled
         * @param factor_x - scaling factor in OX: 1.f == 100% (without changes)
         * @param factor_y - scaling factor in OY: 1.f == 100% (without changes)
         */
        void scale(cv::Mat& sprite, float factor_x, float factor_y);

        /**
         * @brief equivalent of std::clamp for scaling cv::Mat
         * @param sprite - sprite to be scaled
         * @param factor_x - scaling factor in OX: 1.f == 100% (without changes)
         * @param factor_y - scaling factor in OY: 1.f == 100% (without changes)
         * @param lower - lower bound of size
         * @param upper - upper bound of size
         */
        void scale_clamp(cv::Mat& sprite, float factor_x, float factor_y,
                         int lower, int upper);
    };
}  // namespace itl

#endif