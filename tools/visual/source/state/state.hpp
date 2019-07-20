#pragma once

#include <map>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <opencv2/core/utility.hpp>

#include <ThreadPool.h>

#include "../config/config.hpp"
#include "../manager/texture_manager.hpp"
#include "../effect/effect_manager.hpp"

namespace itl
{
    class State
    {
    public:
        int run(const std::string& path_to_pictures, const std::string& extension, const std::string& path_to_data);

        explicit State(const std::string& title);

    private:
        bool load_textures(const std::string& path_to_data) noexcept;
        bool generate_images(const std::string& dir, const std::string& extension);
        bool process_line(const std::string path_to_raw, const std::string dir_to_save,
                                 int background_number, const std::string extension) noexcept;

        std::shared_ptr<TextureManager> texture_manager;
        std::unique_ptr<EffectManager> effect_manager;
        std::unique_ptr<ThreadPool> thread_pool;
        std::queue<std::shared_ptr<sf::RenderWindow>> windows;
        std::mutex mutex;
        const int hardware_concurrency;
        int assigned_threads_to_data;
        using texture_id = int;
        std::map<std::thread::id, texture_id> convert_from_thread_to_texture;
    };
}

