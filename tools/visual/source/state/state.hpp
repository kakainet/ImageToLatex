#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <ThreadPool.h>

#include "../config/config.hpp"
#include "../effect/effect_manager.hpp"
#include "../flag_manager/flag_manager.hpp"
#include "../transform/transform.hpp"

namespace itl
{
    class State
    {
       public:
        int run(const std::string& path_to_data, const std::string& extension);

        explicit State(const std::string& title,
                       const std::shared_ptr<Logger>& log,
                       const std::shared_ptr<itl::FlagManager>& flag_manager);

       private:
        bool generate_images(const std::string& dir,
                             const std::string& extension);
        bool process_line(const std::string& path_to_raw,
                          const std::string& dir_to_save,
                          const std::string& path_to_background,
                          int background_idx,
                          const std::string& extension) noexcept;

        std::shared_ptr<FlagManager> flag_manager;
        std::shared_ptr<Logger> logger;
        std::unique_ptr<EffectManager> effect_manager;
        std::unique_ptr<ThreadPool> thread_pool;
        std::unique_ptr<Transform> transform;
        const int hardware_concurrency;
        std::mutex mtx;
    };
}  // namespace itl
