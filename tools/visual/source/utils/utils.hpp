#pragma once

#include "../math/math.hpp"

#include <vector>

class utils
{
   public:
    template <class T>
    static T& get_random_element(const std::vector<T>& vec);
};

template <class T>
T& utils::get_random_element(const std::vector<T>& vec)
{
    return vec[Math::random(0, static_cast<int>(vec.size() - 1))];
}
