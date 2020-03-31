#include "flag_manager/flag_manager.hpp"
#include "gtest/gtest.h"
#include "logger/logger.hpp"
#include "perlin_noise/perlin_noise.hpp"
#include "state/state.hpp"

int main(int argc, char* argv[])
{
    if(argc < cst::system::required_command_args_size)
    {
        itl::Logger::init(true, true, true, true, true, false);
        itl::Logger::log(cst::system::wrong_args_size,
                    itl::Logger::stream_t::console, itl::Logger::type_t::error);
        itl::Logger::log(cst::system::usage, itl::Logger::stream_t::console,
                    itl::Logger::type_t::info);
        return cst::system::error_code;
    }

    int flag_number = argc - cst::system::required_command_args_size;

    std::shared_ptr<itl::FlagManager> flag_manager =
        std::make_shared<itl::FlagManager>(argc, argv);

    itl::Logger::init(flag_manager->contains_flag(cst::flag::log_all),
                 flag_manager->contains_flag(cst::flag::log_info),
                 flag_manager->contains_flag(cst::flag::log_suggestions),
                 flag_manager->contains_flag(cst::flag::log_erros),
                 flag_manager->contains_flag(cst::flag::log_warnings),
                 flag_manager->contains_flag(cst::flag::log_time));

    if(flag_manager->contains_flag(cst::flag::testing))
    {
        testing::InitGoogleTest(&argc, argv);

        if(RUN_ALL_TESTS() != cst::gtest::tests_passed)
        {
            itl::Logger::log(cst::gtest::fail_msg, itl::Logger::stream_t::console,
                        itl::Logger::type_t::warning);
        }
    }

    itl::State state(flag_manager);
    int result = state.run(argv[flag_number + cst::system::dir_to_data_idx],
                           argv[flag_number + cst::system::extension_idx]);

    itl::Logger::log(result == cst::system::pass_code ? cst::info::finished
                                                 : cst::info::failed,
                itl::Logger::stream_t::console, itl::Logger::type_t::info);

    return result;
}