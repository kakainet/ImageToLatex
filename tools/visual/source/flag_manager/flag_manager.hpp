#pragma once

#include "../config/config.hpp"
#include "../logger/logger.hpp"

#include <algorithm>
#include <string>
#include <memory>
#include <vector>

namespace itl
{
    class FlagManager
    {
    public:
        explicit FlagManager(int argc, char* argv[], const std::shared_ptr<Logger>& log);
        bool containsFlag(const std::string& flag);
    private:
        std::vector<std::string> flags;
        std::shared_ptr<Logger> logger;
    };
}
