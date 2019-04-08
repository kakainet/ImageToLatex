#pragma once

#pragma once

#include "../../external/sfml/include/SFML/System/Vector2.hpp"

namespace constants
{
    namespace window
    {
        const auto size = sf::Vector2f{800,800};
    }

    namespace cache
    {
        constexpr auto empty_path = "Cache can't find resource in empty path.";
        constexpr auto not_found = "Cache can't find resource in this path.";
    }
}