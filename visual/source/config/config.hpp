#pragma once

#pragma once

#include "../../external/sfml/include/SFML/System/Vector2.hpp"

namespace constants
{
    namespace window
    {
        const auto size = sf::Vector2u{800,800};
    }

    namespace system
    {
        constexpr auto command_args_size = 4;
        constexpr auto wrong_args_size = "Wrong number of arguments";
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