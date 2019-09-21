#include <sys/resource.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "perlin_noise/perlin_noise.hpp"
#include "state/state.hpp"
#include "flag_manager/flag_manager.hpp"
#include "logger/logger.hpp"

int main(int argc, char* argv[])
{
    const rlim_t kStackSize = 64 * 1024 * 1024;   // min stack size = 64 MB
    struct rlimit rl{};
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }

    std::shared_ptr<itl::Logger> logger = std::make_shared<itl::Logger>();
    if(argc < constants::system::required_command_args_size)
    {
        logger->log(constants::system::wrong_args_size, itl::Logger::STREAM::BOTH, itl::Logger::TYPE::ERROR);
        logger->log(constants::system::usage, itl::Logger::STREAM::CONSOLE, itl::Logger::TYPE::INFO);
        return constants::system::error_code;
    }

    int flag_number = argc - constants::system::required_command_args_size;

    std::shared_ptr<itl::FlagManager> flag_manager = std::make_shared<itl::FlagManager>(argc, argv, logger);

    logger->init(flag_manager->contains_flag(constants::flags::logging_all),
                 flag_manager->contains_flag(constants::flags::logging_info),
                 flag_manager->contains_flag(constants::flags::logging_suggestions),
                 flag_manager->contains_flag(constants::flags::logging_erros),
                 flag_manager->contains_flag(constants::flags::logging_warnings));

    if(flag_manager->contains_flag(constants::flags::testing))
    {
        testing::InitGoogleTest(&argc, argv);

        if(RUN_ALL_TESTS() != constants::gtest::tests_passed)
        {
            logger->log(constants::gtest::fail_msg, itl::Logger::STREAM::BOTH, itl::Logger::TYPE::WARNING);
        }
    }

    itl::State state("ImageToLatex", logger, flag_manager);
    return state.run(argv[flag_number + constants::system::dir_to_data_idx],
                     argv[flag_number + constants::system::extension_idx]);
}