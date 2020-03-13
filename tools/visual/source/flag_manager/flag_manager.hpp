#ifndef ITL_FLAG_MGR_HPP
#define ITL_FLAG_MGR_HPP

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
    /**
     * class which manages command-line arguments
     */
    class FlagManager
    {
       public:
        /**
         * @brief explicit ctor
         * @param argc - number of arguments
         * @param argv - arguments
         * @param log - ptr to logger
         */
        explicit FlagManager(int argc, char* argv[],
                             const std::shared_ptr<Logger>& log);

        /**
         * @brief checks if given flag was passed to program
         * @param flag - flag in format "-sth"
         * @return true if flag is in manager, false otherwise
         */
        bool contains_flag(const std::string& flag);

       private:
        std::vector<std::string> flags;  ///< vector of caught flags
        std::shared_ptr<Logger> logger;  ///< ptr to logger
    };
}  // namespace itl

#endif