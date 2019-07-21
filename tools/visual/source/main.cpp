#include <SFML/Graphics.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "state/state.hpp"
#include "flag_manager/flag_manager.hpp"
#include "logger/logger.hpp"

int main(int argc, char* argv[])
{
    std::shared_ptr<itl::Logger> logger;

    if(argc < constants::system::required_command_args_size)
    {
        logger->log(constants::system::wrong_args_size, itl::Logger::STREAM::BOTH, itl::Logger::TYPE::ERROR);
        logger->log(constants::system::usage, itl::Logger::STREAM::CONSOLE, itl::Logger::TYPE::INFO);
        return constants::system::error_code;
    }

    itl::FlagManager flagManager(argc, argv, logger);

    if(flagManager.containsFlag(constants::flags::testing))
    {
        testing::InitGoogleTest(&argc, argv);

        if(RUN_ALL_TESTS() != constants::gtest::tests_passed)
        {
            logger->log(constants::gtest::fail_msg, itl::Logger::STREAM::BOTH, itl::Logger::TYPE::WARNING);
        }
    }

    itl::State state("ImageToLatex", logger);
    return state.run(argv[constants::system::dir_to_pics_idx],
                     argv[constants::system::extension_idx],
                     argv[constants::system::dir_to_data_idx]);
}