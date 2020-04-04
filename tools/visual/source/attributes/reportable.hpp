#ifndef ITL_REPORTABLE_HPP
#define ITL_REPORTABLE_HPP

#include "../logger/logger.hpp"

namespace itl
{
    class Reportable
    {
       public:
        /**
         * @brief constructs base and prints module name
         * @param module_name
         */
        Reportable(const std::string& module_name);

        /**
         * @brief default ctor deleted
         */
        Reportable() = delete;
    };
}  // namespace itl

#endif  // ITL_REPORTABLE_HPP