#include <map>
#include <vector>
#include <memory>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../config/config.hpp"
#include "../manager/texture_manager.hpp"
#include "../manager/font_manager.hpp"
#pragma once

namespace itl
{
    class State
    {
    public:

        int run(const std::string& name, const sf::Vector2i& interval);

        State();


    private:
        bool load_fonts() noexcept;
        bool load_textures() noexcept;
        bool generate_images(const std::string& name, const sf::Vector2i& interval);
        bool process_line(const std::string& path_to_raw) noexcept;

        std::shared_ptr<TextureManager> texture_manager;
        std::shared_ptr<FontManager> font_manager;
    };
}
