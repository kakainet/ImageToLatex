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
};