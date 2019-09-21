#include "flag_manager.hpp"

namespace itl
{
    FlagManager::FlagManager(int argc, char** argv, const std::shared_ptr<Logger>& log)
        : logger(log)
    {
        auto contains_flag_c_ptr = [argv, argc](const std::string& flag) -> bool
        {
            for(int i = 0; i < argc; i++)
            {
                if(strcmp(argv[i], flag.c_str()) == 0)
                {
                    return true;
                }
            }
            return false;
        };

        bool print_init = contains_flag_c_ptr(cst::flags::logging_all) ||
                contains_flag_c_ptr(cst::flags::logging_info);

        if(print_init)
        {
            this->logger->log(std::string(cst::info::init_module_msg_start) + std::string(typeid(this).name()),
                              Logger::stream_t::console, Logger::type_t::info);
        }

        for(int i = 0; i < argc - cst::system::required_command_args_size; i++)
        {
            this->flags.emplace_back(std::string(argv[i+1]));
        }

        if(print_init)
        {
            this->logger->log(std::string(cst::info::init_module_msg_end) + std::string(typeid(this).name()),
                              Logger::stream_t::console, Logger::type_t::info);
        }
    }

    bool FlagManager::contains_flag(const std::string& flag)
    {
        return std::find(this->flags.begin(), this->flags.end(), flag) != this->flags.end();
    }
}
