#pragma once

#pragma once

#include "../../external/sfml/include/SFML/System/Vector2.hpp"

namespace constants
{
    namespace window
    {
        const auto size = sf::Vector2f{800,800};
    }

    namespace system
    {
        constexpr auto empty_path = "Cache can't find resource in empty path.";
        constexpr auto not_found = "Cache can't find resource in this path.";
        constexpr auto line_processing_error = "Error occured during line processing";
        constexpr auto latex_path = "../../../latex/latex.sh";
    }

    namespace perlin_noise
    {
        constexpr auto noises_num = 200;
    }
}