#pragma once

#include "../config/config.hpp"
#include "../logger/logger.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace itl
{
    class FlagManager
    {
        std::vector<std::string> flags;

    public:
        explicit FlagManager(int argc, char* argv[]);
        bool containsFlag(const std::string& flag);
    };
}
