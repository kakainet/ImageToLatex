#ifndef ITL_STATE_HPP
#define ITL_STATE_HPP

#include <ThreadPool.h>

#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "../config/config.hpp"
#include "../effect/effect_manager.hpp"
#include "../flag_manager/flag_manager.hpp"
#include "../image_manager/image_manager.hpp"
#include "../transform/transform.hpp"

/**
 * everything which is direct part of itl/visual
 */
namespace itl
{
    /**
     * class designed for managing whole process
     */
    class State
    {
       public:
        /**
         * @brief explicit ctor
         * @param flag_manager - ptr to flag manager
         */
        explicit State(const std::shared_ptr<itl::FlagManager>& flag_manager);

        /**
         * @brief run whole imaging processing
         * @param path_to_data - path to "data" folder
         * @param extension - extension of supported images
         * @return 0 if program finished without problems, 1 otherwise
         */
        int run(const std::string& path_to_data, const std::string& extension);

       private:
        /**
         * @brief generates all images using effects
         * @param dir - path to data folder
         * @param extension - ext of supported images
         * @return
         */
        bool generate_images(const std::string& dir,
                             const std::string& extension);

        /**
         * @brief process 1 image and produces many modified images
         * @param path_to_raw - path to raw input image
         * @param dir_to_save - directory where output should be saved
         * @param background - of latex images
         * @param background_idx - index of background
         * @param extension - extension of supported images (both background and
         * sprites)
         * @return true if everything finished correctly
         */
        bool process_line(const std::string& path_to_raw,
                          const std::string& dir_to_save, cv::Mat background,
                          int background_idx,
                          const std::string& extension) noexcept;

        std::shared_ptr<FlagManager> flag_manager;      ///< ptr to flag manager
        std::shared_ptr<Logger> logger;                 ///< ptr to logger
        std::unique_ptr<EffectManager> effect_manager;  ///< own effect manager
        std::unique_ptr<ThreadPool> thread_pool;  ///< thread pool for workers
        std::unique_ptr<Transform> transform;     ///< transform for images
        std::unique_ptr<ImageManager>
            background_manager;             ///< manager for backgrounds
        const size_t hardware_concurrency;  ///< numbers of used threads
        std::mutex mtx;  ///< own mutex for avoid race condition
    };
}  // namespace itl

#endif