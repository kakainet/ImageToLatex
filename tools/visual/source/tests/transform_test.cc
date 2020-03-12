#include "../transform/transform.hpp"

#include <gtest/gtest.h>

TEST(TransformTest, merge_images)
{
    cv::Mat fst = cv::Mat::zeros(cv::Size(3, 3), CV_8UC4);
    cv::Mat snd = cv::Mat::ones(cv::Size(2, 2), CV_8UC4);
}

TEST(TransformTest, rotate)
{
    std::shared_ptr<itl::Logger> log =
        std::make_shared<itl::Logger>(false, false, false, false, false, false);
    itl::Transform t(log);

    cv::Mat fst = cv::Mat::zeros(cv::Size(50, 50), CV_8UC4);

    EXPECT_EQ(fst.rows, 50);
    EXPECT_EQ(fst.cols, 50);
    t.rotate(fst, 45);
    EXPECT_EQ(fst.rows, 71);
    EXPECT_EQ(fst.cols, 71);

    cv::Mat snd = cv::Mat::ones(cv::Size(50, 50), CV_8UC4);

    EXPECT_EQ(snd.rows, 50);
    EXPECT_EQ(snd.cols, 50);
    t.rotate(snd, 0);
    EXPECT_EQ(snd.rows, 50);
    EXPECT_EQ(snd.cols, 50);
    t.rotate(snd, 5);
    EXPECT_EQ(snd.rows, 54);
    EXPECT_EQ(snd.cols, 54);
    t.rotate(snd, 10);
    EXPECT_EQ(snd.rows, 63);
    EXPECT_EQ(snd.cols, 63);
    t.rotate(snd, 15);
    EXPECT_EQ(snd.rows, 77);
    EXPECT_EQ(snd.cols, 77);
}

TEST(TransformTest, move)
{
    std::shared_ptr<itl::Logger> log =
        std::make_shared<itl::Logger>(false, false, false, false, false, false);
    itl::Transform t(log);
    auto compare = [](const cv::Mat& a, const cv::Mat& b) -> bool {
        return cv::countNonZero(a != b) == 0;
    };
    cv::Mat fst = cv::Mat::ones(cv::Size(10, 10), CV_8UC4);
    EXPECT_EQ(fst.rows, 10);
    EXPECT_EQ(fst.cols, 10);
    EXPECT_TRUE(compare(fst, cv::Mat::ones(cv::Size(10, 10), CV_8UC4)));
    t.move(fst, 500, 500);
    EXPECT_EQ(fst.rows, 10);
    EXPECT_EQ(fst.cols, 10);
    EXPECT_TRUE(!compare(fst, cv::Mat::ones(cv::Size(10, 10), CV_8UC4)));
    EXPECT_TRUE(compare(fst, cv::Mat::zeros(cv::Size(10, 10), CV_8UC4)));

    cv::Mat snd = cv::Mat::ones(cv::Size(2, 2), CV_8UC4);
    EXPECT_EQ(snd.rows, 2);
    EXPECT_EQ(snd.cols, 2);
    EXPECT_TRUE(compare(snd, cv::Mat::ones(cv::Size(2, 2), CV_8UC4)));
    t.move(snd, 1, 0);
    EXPECT_TRUE(!compare(snd, cv::Mat::ones(cv::Size(2, 2), CV_8UC4)));
    float data[4] = {0, 0, 1, 1};
    EXPECT_TRUE(compare(snd, cv::Mat(2, 2, CV_8UC4, &data)));
    t.move(snd, 0, 1);
    float data_corner[4] = {0, 0, 0, 1};
    EXPECT_TRUE(compare(snd, cv::Mat(2, 2, CV_8UC4, &data_corner)));
}

TEST(TransformTest, scale) {}

TEST(TransformTest, scale_clamp) {}
