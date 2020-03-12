#pragma once

#include <opencv2/core.hpp>
#include <vector>

/**
 * constants
 */
namespace cst
{
    namespace window
    {
        constexpr int dim = 64;  // ==> size dim x dim
    }

    namespace image
    {
        constexpr uint64_t img_mgr_reserve = 8;
    }

    namespace system
    {
        constexpr char const* usage =
            "Usage: \n\t| <optional> flags \n\t| path/to/data/ \n\t| extension "
            "of "
            "pictures";
        constexpr int8_t dir_to_data_idx = 1;
        constexpr int8_t extension_idx = 2;
        constexpr int8_t required_command_args_size = 3;
        constexpr char const* wrong_args_size = "Wrong number of arguments.";
        constexpr char const* empty_path =
            "Cache can't find resource in empty path.";
        constexpr char const* not_found =
            "Cache can't find resource in this path.";
        constexpr char const* line_processing_error =
            "Error occured during line processing.";
        constexpr int8_t error_code = 1;
        constexpr int8_t pass_code = 0;
    }  // namespace system

    namespace file
    {
        constexpr char const* noise = "noises/";
        constexpr char const* pic = "pics/";
        constexpr char const* logger = "log/";
        constexpr char const* texture = "textures/";
        constexpr char const* input = "input/";
        constexpr char const* output = "output/";
    }  // namespace file

    namespace info
    {
        constexpr char const* init_module_msg_start = "Initializing module: ";
        constexpr char const* init_module_msg_end = "Initialized module: ";
        constexpr char const* finished = "Work finished successfully";
        constexpr char const* failed = "Something went wrong";

    }  // namespace info

    namespace gtest
    {
        constexpr int8_t tests_passed = 0;
        constexpr int8_t tests_failed = 1;
        constexpr char const* fail_msg =
            "Some unit tests failed. Program may not work correctly.";
    }  // namespace gtest

    namespace effect
    {
        const cv::Vec2f scale_incr_bounds = {1.f, 1.3f};
        const cv::Vec2f scale_decr_bounds = {0.7f, 1.f};
        constexpr float max_degree = 15.f;
        constexpr int accuracy = 100;
        constexpr int8_t rgb_channel_idx = 3;
        constexpr int8_t rgba_channel_idx = 4;
        constexpr char const* wrong_channel =
            "Failed during recognizing channel of input matrix";
        constexpr float alpha_trash_hold = 10.f;
        constexpr char const* failed_merging =
            "Failed during creating roi: out of image boundaries";
    }  // namespace effect

    namespace flag
    {
        constexpr char const* contradiction_flags =
            "Flags which were passed to program are contradictory.";
        constexpr char const* testing = "-t";
        constexpr char const* printing_steps = "-p";
        constexpr char const* log_erros = "-le";
        constexpr char const* log_info = "-li";
        constexpr char const* log_suggestions = "-ls";
        constexpr char const* log_warnings = "-lw";
        constexpr char const* log_all = "-la";
        constexpr char const* log_time = "-lt";
    }  // namespace flag

    namespace color
    {
        constexpr char const* reset = "\033[0m";
        constexpr char const* bold_red = "\033[1;31m";
        constexpr char const* bold_yellow = "\033[1;33m";
        constexpr char const* bold_cyan = "\033[1;36m";
        constexpr char const* bold_magenta = "\033[1;37m";
        constexpr char const* bold_green = "\033[1;92m";
    }  // namespace color

    namespace manager
    {
        constexpr char const* loading_data_second_time =
            "Tried to load data second time";
        constexpr char const* failed_update = "Failed during updating texture";
        constexpr char const* data_path_empty = "Data path is empty";
    }  // namespace manager

    namespace texture
    {
        constexpr char const* failed_load_texture = "Failed to load texture.";
    }

    namespace thread
    {
        constexpr char const* fail_distr_worker =
            "Failed during distribution worker - there is no free worker.";
        constexpr char const* number_thread_info =
            "Program use multithreading. Threads number:";
    }  // namespace thread

    namespace perlin
    {
        constexpr int dim = 256;
        constexpr int size = dim * dim;
        constexpr int permutation_size = 256;
        constexpr double lower_bound = 0.6;
        constexpr double upper_bound = 1.0;
        constexpr double start_delta = 0.2;
        constexpr float frequency = 10;
        constexpr char const* missing_noise =
            "Missing noise. Program will recreate component with id ";
        constexpr int noises_num = 2;
    }  // namespace perlin
}  // namespace cst