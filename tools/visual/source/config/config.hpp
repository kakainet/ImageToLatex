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

    namespace perlin_noise
    {
        constexpr auto noises_num = 20;
        constexpr auto hash = {208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
                               185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
                               9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
                               70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
                               203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
                               164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
                               228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
                               232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
                               193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
                               101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
                               135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
                               114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219};
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
}