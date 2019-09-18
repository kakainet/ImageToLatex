#pragma once

#include <vector>

#include <opencv2/core.hpp>

namespace constants
{
    namespace window
    {
        const auto size = cv::Vec2i{800,800};
    }

    namespace system
    {
        constexpr auto usage = "Usage: \n\t| <optional> flags \n\t| DIR_TO_PICTURES \n\t| EXTENSION_OF_PICTURES \n\t| DIR_TO_DATA";
        constexpr auto dir_to_pics_idx = 1;
        constexpr auto extension_idx = 2;
        constexpr auto dir_to_data_idx = 3;
        constexpr auto required_command_args_size = 4;
        constexpr auto wrong_args_size = "Wrong number of arguments.";
        constexpr auto empty_path = "Cache can't find resource in empty path.";
        constexpr auto not_found = "Cache can't find resource in this path.";
        constexpr auto line_processing_error = "Error occured during line processing.";
        constexpr auto latex_path = "../../../latex/latex.sh";
        constexpr auto error_code = 1;
        constexpr auto pass_code = 0;
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
        const cv::Vec2f scale_incr_bounds = {1.05f, 2.f};
        const cv::Vec2f scale_decr_bounds = {0.5f, 0.95f};
        constexpr auto max_degree = 45;
        constexpr auto accuracy = 100;
        constexpr auto rgb_channel_idx = 3;
        constexpr auto rgba_channel_idx = 4;
        constexpr auto wrong_channel = "Failed during recognizing channel of input matrix";
        constexpr auto alpha_trash_hold = 10;
        constexpr auto failed_merging = "Failed during creating roi: out of image boundaries";
    }

    namespace flags
    {
        constexpr auto contradiction_flags = "FLags which were passed to program are contradictory.";
        constexpr auto testing = "-t";
        constexpr auto printing_steps = "-p";
        constexpr auto logging_erros = "-le";
        constexpr auto logging_info = "-li";
        constexpr auto logging_suggestions = "-ls";
        constexpr auto logging_warnings = "-lw";
        constexpr auto logging_all = "-la";
    }

    namespace color
    {
        constexpr auto reset = "\033[0m";
        constexpr auto bold_red = "\033[1;31m";
        constexpr auto bold_yellow = "\033[1;33m";
        constexpr auto bold_cyan = "\033[1;36m";
        constexpr auto bold_magenta = "\033[1;37m";
        constexpr auto bold_green = "\033[1;92m";
    }

    namespace manager
    {
        constexpr auto loading_data_second_time = "Tried to load data second time";
        constexpr auto failed_update = "Failed during updating texture";
        constexpr auto data_path_empty = "Data path is empty";
    }

    namespace texture
    {
        constexpr auto failed_load_texture = "Failed to load texture.";
    }

    namespace thread
    {
        constexpr auto fail_distr_worker = "Failed during distribution worker - there is no free worker.";
        constexpr auto number_thread_info = "Program use multithreading. Threads number:";
    }

    namespace perlin
    {
        constexpr int dim = 16;
        constexpr int size = dim * dim;
        constexpr int permutation_size = 256;
        constexpr double lower_bound = 0.6;
        constexpr double upper_bound = 1.0;
        constexpr double start_delta = 0.2;
        constexpr auto frequency = 10;
        constexpr auto missing_noise = "Missing noise. Program will recreate component with id ";
        constexpr auto noises_num = 20;
    }
}