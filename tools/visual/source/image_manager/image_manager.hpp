#ifndef ITL_IMAGE_MGR_HPP
#define ITL_IMAGE_MGR_HPP

#include <numeric>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

#include "../logger/logger.hpp"

namespace itl
{
    /**
     * class for storing & managing iamges
     */
    class ImageManager
    {
       public:
        /**
         * @brief classic ctor
         */
        ImageManager();

        /**
         * @brief appends image to manager
         * @param path to image
         * @return true if given path represents proper image
         */
        bool append(const std::string& path);

        /**
         * @brief appends images to manager
         * @param paths to images
         * @return true if given paths represent proper image
         */
        bool append(const std::vector<std::string>& paths);

        /**
         * @brief [] operator for stored images
         * @param idx - index of image
         * @return idx'th image
         */
        cv::Mat& operator[](int idx);

       private:
        std::vector<cv::Mat> imgs;       ///< vector of cv::Mat
        std::shared_ptr<Logger> logger;  ///< ptr to logger
    };
}  // namespace itl

#endif