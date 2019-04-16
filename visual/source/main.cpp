#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "state/state.hpp"

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    (void)RUN_ALL_TESTS();
    itl::State state;
    return state.run();
}