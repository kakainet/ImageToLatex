#include "reportable.hpp"

namespace itl
{
    Reportable::Reportable()
    {
        Logger::log(std::string(cst::info::init_module_msg_start) +
                          std::string(typeid(this).name()),
                          Logger::stream_t::console, Logger::type_t::info);
    }
}  // namespace itl
