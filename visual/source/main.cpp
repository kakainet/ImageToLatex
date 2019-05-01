#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "state/state.hpp"
#include "config/config.hpp"
#include "logger/logger.hpp"

int main(int argc, char* argv[])
{
    if(argc != constants::system::command_args_size)
    {
        itl::Logger::Log(constants::system::wrong_args_size);
        return 0;
    }

    testing::InitGoogleTest(&argc, argv);
    (void)RUN_ALL_TESTS();
    itl::State state;
    return state.run(argv[0], sf::Vector2i{atoi(argv[1]), atoi(argv[2])});
}