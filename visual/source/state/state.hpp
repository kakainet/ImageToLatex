#include <map>
#include <vector>
#include <memory>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <opencv2/core/utility.hpp>

#include "../config/config.hpp"
#include "../manager/texture_manager.hpp"
#include "../manager/font_manager.hpp"
#include "../effect/EffectManager.hpp"

#pragma once

namespace itl
{
    class State
    {
    public:
        std::shared_ptr<sf::RenderWindow> window;
        sf::Sprite background;
        int run(const std::string& path_to_pictures, const std::string& extension, const std::string& path_to_data);

        explicit State(const std::string& title);

    private:
        bool load_fonts(const std::string& path_to_data) noexcept;
        bool load_textures(const std::string& path_to_data) noexcept;
        bool generate_images(const std::string& dir, const std::string& extension);
        bool process_line(const std::string& path_to_raw, const std::string& dir_to_save,
                const std::string& background_number, const std::string& extension) noexcept;

        std::shared_ptr<TextureManager> texture_manager;
        std::shared_ptr<FontManager> font_manager;
        std::unique_ptr<EffectManager> effect_manager;
    };
}

