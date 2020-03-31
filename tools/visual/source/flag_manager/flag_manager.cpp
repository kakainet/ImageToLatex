#include "flag_manager.hpp"

namespace itl
{
    FlagManager::FlagManager(int argc, char** argv)
    {
        for(int i = 0; i < argc - cst::system::required_command_args_size; i++)
        {
            this->flags.insert(std::string(argv[i + 1]));
        }
    }

    bool FlagManager::contains_flag(const std::string& flag)
    {
        return flags.find(flag) != flags.end();
    }
}  // namespace itl
