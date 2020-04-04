#include "transform.hpp"

namespace itl
{
    Transform::Transform() : Reportable(typeid(this).name()) {}

    void Transform::merge_images(cv::Mat* background, cv::Mat* upcoming, int x,
                                 int y)
    {
        auto handle_cv_8uc4 = [=](int i, int j) {
            if(upcoming->at<cv::Vec4b>(j, i)[3] > cst::effect::alpha_trash_hold)
            {
                background->at<cv::Vec4b>(y + j, x + i) =
                    upcoming->at<cv::Vec4b>(j, i);
            }
        };

        auto handle_cv_8uc3 = [=](int i, int j) {
            background->at<cv::Vec4b>(y + j, x + i)[0] =
                upcoming->at<cv::Vec3b>(j, i)[0];
            background->at<cv::Vec4b>(y + j, x + i)[1] =
                upcoming->at<cv::Vec3b>(j, i)[1];
            background->at<cv::Vec4b>(y + j, x + i)[2] =
                upcoming->at<cv::Vec3b>(j, i)[2];
            background->at<cv::Vec4b>(y + j, x + i)[3] =
                std::numeric_limits<uint8_t>::max() - 1;
        };

        for(int i = 0; i < upcoming->cols; i++)
        {
            for(int j = 0; j < upcoming->rows; j++)
            {
                if(j + y >= background->rows)
                {
                    break;
                }

                if(x + i >= background->cols)
                {
                    return;
                }

                switch(upcoming->channels())
                {
                    case cst::effect::rgb_channel_idx:
                    {
                        handle_cv_8uc3(i, j);
                        break;
                    }

                    case cst::effect::rgba_channel_idx:
                    {
                        handle_cv_8uc4(i, j);
                        break;
                    }

                    default:
                    {
                        Logger::get()->log(cst::effect::failed_merging,
                                           Logger::stream_t::console,
                                           Logger::type_t::error);
                        return;
                    }
                }
            }
        }
    }

    void Transform::rotate(cv::Mat& sprite, float angle)
    {
        cv::Point2f center((sprite.cols - 1) / 2.f, (sprite.rows - 1) / 2.f);
        cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
        cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), sprite.size(), angle)
                              .boundingRect2f();
        rot.at<double>(0, 2) += bbox.width / 2.0 - sprite.cols / 2.0;
        rot.at<double>(1, 2) += bbox.height / 2.0 - sprite.rows / 2.0;

        cv::Mat dst;
        cv::warpAffine(sprite, dst, rot, bbox.size());
        sprite = dst;
    }

    void Transform::move(cv::Mat& sprite, float pixels_dx, float pixels_dy)
    {
        cv::Mat trans_mat =
            (cv::Mat_<double>(2, 3) << 1, 0, pixels_dx, 0, 1, pixels_dy);
        cv::warpAffine(sprite, sprite, trans_mat, sprite.size());
    }

    void Transform::scale(cv::Mat& sprite, float factor_x, float factor_y)
    {
        cv::resize(sprite, sprite,
                   cv::Size(static_cast<int>(sprite.cols * factor_x),
                            static_cast<int>(sprite.rows * factor_y)),
                   0, 0, cv::INTER_LINEAR);
    }

    void Transform::scale_clamp(cv::Mat& sprite, float factor_x, float factor_y,
                                int lo, int hi)
    {
        float dx, dy;
        int current_x = static_cast<int>(factor_x * sprite.cols);
        int current_y = static_cast<int>(factor_y * sprite.rows);
        if(current_x <= hi)
        {
            dx = current_x >= lo ? factor_x
                                 : static_cast<float>(lo) / sprite.cols;
        }
        else
        {
            dx = static_cast<float>(hi) / sprite.cols;
        }

        if(current_y <= hi)
        {
            dy = current_y >= lo ? factor_x
                                 : static_cast<float>(lo) / sprite.rows;
        }
        else
        {
            dy = static_cast<float>(hi) / sprite.rows;
        }

        scale(sprite, dx, dy);
    }

}  // namespace itl