#include "flag_manager.hpp"

namespace itl
{
    FlagManager::FlagManager(int argc, char** argv, const std::shared_ptr<Logger>& log)
        : logger(log)
    {
        for(int i = 0; i < argc - constants::system::required_command_args_size; i++)
        {
            this->flags.emplace_back(std::string(argv[i+1]));
        }
    }

    bool FlagManager::containsFlag(const std::string& flag)
    {
        return std::find(this->flags.begin(), this->flags.end(), flag) != this->flags.end();
    }
}
