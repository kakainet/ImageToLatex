#include "state.hpp"

namespace itl
{
    State::State()
    {
        this->font_manager = std::make_shared<FontManager>();
        this->texture_manager = std::make_shared<TextureManager>();
        this->window = std::make_shared<sf::RenderWindow>(
                new sf::RenderWindow(
                        sf::VideoMode( constants::window::size.x, constants::window::size.y ),
                        title) );
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
        sf::Texture& current_texture = this->texture_manager->get_random_element();
        sf::Texture raw_tex;
        sf::Sprite raw_tex_sprite;
        if(!raw_tex.loadFromFile(path_to_raw))
        {
            //logger
        }
        this->background->setTexture(current_texture);
        //this->background->setGlobalBounds(center) //temporary removed
        this->background->setPosition(constants::window::size/2.f);
        this->raw_tex_sprite.setTexture(raw_tex);
        sf::Vector2f pos = {random(raw_tex_sprite.getGlobalBounds().width,
                            constants::window::size.x-raw_tex_sprite.getGlobalBounds().width),
                            random(raw_tex_sprite.getGlobalBounds().height,
                                   constants::window::size.y-raw_tex_sprite.getGlobalBounds().height),
                            };
        this->raw_tex_sprite.setPosition(pos);
        window->display();
        //copy screen
        return true;
    }
}

