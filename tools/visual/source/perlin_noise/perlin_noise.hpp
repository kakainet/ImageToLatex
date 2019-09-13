/*
 * Perlin Noise class
 * based on paper:
 * ----------------
 * Improving Noise
 * Ken Perlin
 * Media Research Laboratory, Dept. of Computer Science, New York University
 * perlin@cat.nyu.edu
 * ----------------
 * and proposed code related with that paper:
 * ----------------
 * https://mrl.nyu.edu/~perlin/noise/
 * created by KEN PERLIN (2002)
 * ----------------
 */

#pragma once

#include <algorithm>
#include <random>
#include <vector>
#include <string>
#include <memory>

#include <boost/filesystem.hpp>

#include "../config/config.hpp"
#include "../logger/logger.hpp"
#include "../math/math.hpp"

namespace itl
{
    class PerlinNoise
    {
    public:
        explicit PerlinNoise(const std::shared_ptr<Logger>& log,
                             const std::string& path_to_data);
        float** get_random_noise() const;
    private:
        void generate_noise(const std::string& dir, int idx) const;
        float generate_point_noise(int x, int y, int z, const std::array<int, constants::perlin::size>& permutation) const;
        void save_to_file(const cv::Mat& map, const std::string& dir) const;
        void read_from_file(const std::string& dir);

        std::vector<float*> noises;
        std::shared_ptr<Logger> logger;
    };
}