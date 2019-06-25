#include "state.hpp"

namespace itl
{
    State::State(const std::string& title)
    {
        this->font_manager = std::make_shared<FontManager>();
        this->texture_manager = std::make_shared<TextureManager>();
        this->window = std::make_shared<sf::RenderWindow>(sf::VideoMode( constants::window::size.x, constants::window::size.y ), title);
        this->background = std::make_unique<sf::Sprite>();
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
            std::string line = name+std::to_string(i)+".png";
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
        window->clear();

        window->display();
        //copy screen
        return true;
    }
}

