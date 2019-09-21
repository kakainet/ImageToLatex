#pragma once
#include <random>

/*
 * @brief class which is used to compute more complex things
 */
class Math
{
public:

  /*
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

    /*
     * @brief random value from "from" to "to"
     * @param from [from ...
     * @param to [... to]
     * @param accuracy of random (i.e 1 for integral parts, 10 for decimal parts etc)
     * @return that value
     */
    static float random_float(float from, float to, int accuracy)
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dis(static_cast<int>(from*accuracy), static_cast<int>(to*accuracy));

      return (float)dis(gen)/accuracy;
    }

    static double fade(double t) noexcept
    {
      return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static double lerp(double t, double a, double b) noexcept
    {
      return a + t * (b - a);
    }

    static double grad(uint8_t hash, double x, double y, double z) noexcept
    {
      int h = hash & 15;
      float u = h < 8 ? x : y;
      float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
      return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};