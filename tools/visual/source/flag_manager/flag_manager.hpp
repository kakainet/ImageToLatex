#ifndef ITL_FLAG_MGR_HPP
#define ITL_FLAG_MGR_HPP

#include <string.h>

#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_set>

#include "../config/config.hpp"
#include "../logger/logger.hpp"

namespace itl
{
    /**
     * class which manages command-line arguments
     * @warning dispensed with the obligation to logging during initialization
     * itself
     */
    class FlagManager
    {
       public:
        /**
         * @brief explicit ctor
         * @param argc - number of arguments
         * @param argv - arguments
         */
        explicit FlagManager(int argc, char* argv[]);

        /**
         * @brief checks if given flag was passed to program
         * @param flag - flag in format "-sth"
         * @return true if flag is in manager, false otherwise
         */
        bool contains_flag(const std::string& flag);

       private:
        std::unordered_set<std::string> flags;  ///< vector of caught flags
    };
}  // namespace itl

#endif