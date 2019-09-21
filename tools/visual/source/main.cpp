#include <sys/resource.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "flag_manager/flag_manager.hpp"
#include "logger/logger.hpp"
#include "perlin_noise/perlin_noise.hpp"
#include "state/state.hpp"

int main(int argc, char* argv[])
{
    std::shared_ptr<itl::Logger> logger = std::make_shared<itl::Logger>();
    if(argc < cst::system::required_command_args_size)
    {
        logger->log(cst::system::wrong_args_size,
                    itl::Logger::stream_t::console, itl::Logger::type_t::error);
        logger->log(cst::system::usage, itl::Logger::stream_t::console,
                    itl::Logger::type_t::info);
        return cst::system::error_code;
    }

    int flag_number = argc - cst::system::required_command_args_size;

    std::shared_ptr<itl::FlagManager> flag_manager =
        std::make_shared<itl::FlagManager>(argc, argv, logger);

    logger->init(flag_manager->contains_flag(cst::flag::log_all),
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
            logger->log(cst::gtest::fail_msg, itl::Logger::stream_t::console,
                        itl::Logger::type_t::warning);
        }
    }

    itl::State state(logger, flag_manager);
    return state.run(argv[flag_number + cst::system::dir_to_data_idx],
                     argv[flag_number + cst::system::extension_idx]);
}