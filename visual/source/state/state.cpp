#include "state.hpp"

namespace itl
{
    State::State()
    {
        this->font_manager = std::make_shared<FontManager>();
        this->texture_manager = std::make_shared<TextureManager>();
    }

    int State::run(const std::string& name, const sf::Vector2i& interval)
    {
        return
                this->load_fonts()
                ||this->load_textures()
                ||this->generate_images(name, interval)
                ? 1 : 0;
    }

    bool State::load_fonts() noexcept
    {
        return this->font_manager->load_data();
    }

    bool State::load_textures() noexcept
    {
        return this->texture_manager->load_data();
    }

    bool State::generate_images(const std::string& name, const sf::Vector2i& interval)
    {
        for(int i = interval.x; i <= interval.y; i++)
        {
            std::string line = name+std::to_string(i);
            if(!this->process_line(line))
            {
                Logger::Log(constants::system::line_processing_error);
                return false;
            }
        }

        return true;
    }

    bool State::process_line(const std::string& path_to_raw) noexcept
    {
        sf::Texture& current_texture = this->texture_manager->get_random_element();

        return true;
    }
}

