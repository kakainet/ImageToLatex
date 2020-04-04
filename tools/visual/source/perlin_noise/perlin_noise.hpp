#ifndef ITL_PERLIN_HPP
#define ITL_PERLIN_HPP

#include <algorithm>
#include <boost/filesystem.hpp>
#include <cmath>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "../attributes/reportable.hpp"
#include "../config/config.hpp"
#include "../logger/logger.hpp"
#include "../math/math.hpp"

namespace itl
{
    /**
     * Perlin Noise class
     * based on paper:
     *
     * Improving Noise
     * Ken Perlin
     * Media Research Laboratory, Dept. of Computer Science, New York University
     * perlin@cat.nyu.edu
     * https://mrl.nyu.edu/~perlin/paper445.pdf
     * and proposed code related with that paper:
     * https://mrl.nyu.edu/~perlin/noise/
     * created by KEN PERLIN (2002)
     *
     */
    class PerlinNoise : public Reportable
    {
       public:
        /**
         * @brief explicit ctor
         * @param path_to_data - path to data folder
         */
        explicit PerlinNoise(const std::string& path_to_data);

        /**
         * @brief takes 1 from predefined noises mask
         * @return random noise mask
         */
        [[nodiscard]] const std::vector<double>& get_random_noise() const;

       private:
        /**
         * @brief generates noise 2d and add it to member vector
         * @param dir - directory to save
         */
        void generate_noise_2d(const std::string& dir);

        /**
         * @brief generates noise in given point
         * @param x - first coordinate
         * @param y - second coordinate
         * @param z - third coordinate
         * @param permutation - permutation needed to perlin noise algorithm
         * @return noise in given point
         */
        [[nodiscard]] double generate_point_noise(
            double x, double y, double z,
            const std::vector<int>& permutation) const;

        /**
         * @brief saves perlin to directory
         * @param noise - produced noise
         * @param dir - given directory
         */
        void save_to_file(const std::vector<double>& noise,
                          const std::string& dir) const;

        /**
         * @brief reads noise from file
         * @param dir - noise directory
         */
        void read_from_file(const std::string& dir);

        std::vector<std::vector<double>> noises;  ///< stored noises
    };
}  // namespace itl

#endif