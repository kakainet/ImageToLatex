#include "reportable.hpp"

namespace itl
{
    Reportable::Reportable(const std::string& module_name)
    {
        Logger::log(std::string(cst::info::init_module_msg_start) +
                        std::string(module_name),
                    Logger::stream_t::console, Logger::type_t::info);
    }
}  // namespace itl
