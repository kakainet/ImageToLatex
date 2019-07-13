#include "flag_manager.hpp"

FlagManager::FlagManager(int argc, char** argv)
{
    for(int i = constants::system::required_command_args_size; i < argc; i++)
    {
        this->flags.emplace_back(argv[i]);
    }
}

bool FlagManager::containsFlag(const std::string& flag)
{
    return std::find(this->flags.begin(), this->flags.end(), flag) != this->flags.end();
}
