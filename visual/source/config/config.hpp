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

    namespace info
    {
        constexpr auto init_module_msg_start = "Initializing module: ";
        constexpr auto init_module_msg_end = "Initialized module: ";
    }

    namespace gtest
    {
        constexpr auto tests_passed = 0;
        constexpr auto tests_failed = 1;
        constexpr auto fail_msg = "Some unit tests failed. Program may not work correctly.";
    }

    namespace effect
    {
        const sf::Vector2f scale_incr_bounds = {1.05f, 2.f};
        const sf::Vector2f scale_decr_bounds = {0.5f, 0.95f};
        constexpr auto accuracy = 100;
    }
}