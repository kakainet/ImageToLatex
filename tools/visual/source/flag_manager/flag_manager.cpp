#include "flag_manager.hpp"

namespace itl
{
    FlagManager::FlagManager(int argc, char** argv)
    {
        Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                    Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

        for(int i = constants::system::required_command_args_size; i < argc; i++)
        {
            this->flags.emplace_back(argv[i]);
        }

        Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                    Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

    }

    bool FlagManager::containsFlag(const std::string& flag)
    {
        return std::find(this->flags.begin(), this->flags.end(), flag) != this->flags.end();
    }
}
