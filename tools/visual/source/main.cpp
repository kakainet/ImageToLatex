#include <SFML/Graphics.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "state/state.hpp"
#include "flag_manager/flag_manager.hpp"
#include "logger/logger.hpp"

int main(int argc, char* argv[])
{
    std::shared_ptr<itl::Logger> logger = std::make_shared<itl::Logger>();

    if(argc < constants::system::required_command_args_size)
    {
        logger->log(constants::system::wrong_args_size, itl::Logger::STREAM::BOTH, itl::Logger::TYPE::ERROR);
        logger->log(constants::system::usage, itl::Logger::STREAM::CONSOLE, itl::Logger::TYPE::INFO);
        return constants::system::error_code;
    }

    int flag_number = argc - constants::system::required_command_args_size;

    itl::FlagManager flag_manager(argc, argv, logger);

    logger->init(flag_manager.contains_flag(constants::flags::logging_all),
                 flag_manager.contains_flag(constants::flags::logging_info),
                 flag_manager.contains_flag(constants::flags::logging_suggestions),
                 flag_manager.contains_flag(constants::flags::logging_erros),
                 flag_manager.contains_flag(constants::flags::logging_warnings));

    if(flag_manager.contains_flag(constants::flags::testing))
    {
        testing::InitGoogleTest(&argc, argv);

        if(RUN_ALL_TESTS() != constants::gtest::tests_passed)
        {
            logger->log(constants::gtest::fail_msg, itl::Logger::STREAM::BOTH, itl::Logger::TYPE::WARNING);
        }
    }

    itl::State state("ImageToLatex", logger);
    return state.run(argv[flag_number + constants::system::dir_to_pics_idx],
                     argv[flag_number + constants::system::extension_idx],
                     argv[flag_number + constants::system::dir_to_data_idx]);
}