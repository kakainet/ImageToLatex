#pragma once
#include <random>

/**
 * class which is used to compute math things
 */
class Math
{
   public:
    /**
     * @brief random value from "from" to "to"
     * @param from [from ...
     * @param to [... to]
     * @return that value
     */
    static int random(int from, int to)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(from, to);

        return dis(gen);
    }

    /**
     * @brief random value from "from" to "to"
     * @param from [from ...
     * @param to [... to]
     * @param accuracy of random (i.e 1 for integral parts, 10 for decimal parts
     * etc)
     * @return that value
     */
    static float random_float(float from, float to, int accuracy)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(static_cast<int>(from * accuracy),
                                            static_cast<int>(to * accuracy));

        return (float)dis(gen) / accuracy;
    }

    /**
     * @brief fade function proposed in https://mrl.nyu.edu/~perlin/paper445.pdf
     * @param t - value
     * @return value after fading
     */
    static double fade(double t) noexcept
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    /**
     * @brief linear interpolation
     * @param t - linear coefficient
     * @param a - value_0
     * @param b - value_1
     * @return lerp value
     */
    static double lerp(double t, double a, double b) noexcept
    {
        return a + t * (b - a);
    }

    /**
     * @brief grad function proposed in https://mrl.nyu.edu/~perlin/paper445.pdf
     * @param hash - value from 0 to 255
     * @param x - first coordinate
     * @param y - second coordinate
     * @param z - third coordinate
     * @return grad value
     */
    static double grad(uint8_t hash, double x, double y, double z) noexcept
    {
        int h = hash & 15;
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};