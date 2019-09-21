#pragma once

#include <string.h>
#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include "../config/config.hpp"
#include "../logger/logger.hpp"

namespace itl
{
    class FlagManager
    {
       public:
        explicit FlagManager(int argc, char* argv[],
                             const std::shared_ptr<Logger>& log);
        bool contains_flag(const std::string& flag);

       private:
        std::vector<std::string> flags;
        std::shared_ptr<Logger> logger;
    };
}  // namespace itl
