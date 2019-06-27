#pragma once

#include "../config/config.hpp"

#include <algorithm>
#include <string>
#include <vector>

class FlagManager
{
    std::vector<std::string> flags;

public:
    explicit FlagManager(int argc, char* argv[]);
    bool containsFlag(const std::string& flag);
};
