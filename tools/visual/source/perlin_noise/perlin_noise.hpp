#pragma once

#include <vector>
#include <string>
#include <memory>

#include <filesystem>

#include "../config/config.hpp"
#include "../logger/logger.hpp"

namespace itl
{
    class PerlinNoise
    {
    public:
        explicit PerlinNoise(const std::shared_ptr<Logger>& log);
    private:
        void generate(const std::string& dir) const noexcept;
        void save_to_file(const cv::Mat& map, const std::string& dir) const;
        void read_from_file(const std::string& dir);

        std::vector<cv::Mat> noises;
        std::shared_ptr<Logger> logger;
    };
}