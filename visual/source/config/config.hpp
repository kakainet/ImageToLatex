#pragma once

#include "../../external/sfml/include/SFML/System/Vector2.hpp"
#include <vector>

namespace constants
{
    namespace window
    {
        const auto size = sf::Vector2u{800,800};
    }

    namespace system
    {
        constexpr auto usage = "Usage: DIR_TO_PICTURES | EXTENSION_OF_PICTURES | DIR_TO_DATA | <optional> flags";
        constexpr auto dir_to_pics_idx = 1;
        constexpr auto extension_idx = 2;
        constexpr auto dir_to_data_idx = 3;
        constexpr auto required_command_args_size = 4;
        constexpr auto wrong_args_size = "Wrong number of arguments";
        constexpr auto empty_path = "Cache can't find resource in empty path.";
        constexpr auto not_found = "Cache can't find resource in this path.";
        constexpr auto line_processing_error = "Error occured during line processing";
        constexpr auto latex_path = "../../../latex/latex.sh";
        constexpr auto error_code = 1;
        constexpr auto pass_code = 0;
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
        constexpr auto max_degree = 45;
        constexpr auto accuracy = 100;
    }

    namespace flags
    {
        constexpr auto testing = "-t";
        constexpr auto printing_steps = "-p";
        constexpr auto logging_erros = "-le";
        constexpr auto logging_all = "-la";
    }

    namespace texture
    {
        constexpr auto failed_load_texture = "Failed to load texture";
    }
}