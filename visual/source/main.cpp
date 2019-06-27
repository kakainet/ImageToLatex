#include <SFML/Graphics.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "state/state.hpp"
#include "flag_manager/flag_manager.hpp"
#include "logger/logger.hpp"

int main(int argc, char* argv[])
{
    if(argc < constants::system::required_command_args_size)
    {
        itl::Logger::Log(constants::system::wrong_args_size, itl::Logger::STREAM::BOTH, itl::Logger::TYPE::ERROR);
        itl::Logger::Log(constants::system::usage, itl::Logger::STREAM::CONSOLE, itl::Logger::TYPE::INFO);
        return 0;
    }

    FlagManager flagManager(argc, argv);

    if(flagManager.containsFlag(constants::flags::testing))
    {
        testing::InitGoogleTest(&argc, argv);

        if(RUN_ALL_TESTS() != constants::gtest::tests_passed)
        {
            itl::Logger::Log(constants::gtest::fail_msg, itl::Logger::STREAM::BOTH, itl::Logger::TYPE::WARNING);
        }
    }

    itl::State state("ImageToLatex");
    return state.run(argv[1], argv[2], argv[3]);
}