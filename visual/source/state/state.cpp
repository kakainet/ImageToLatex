#include "state.hpp"

namespace itl
{
    State::State(const std::string& title)
    {
        itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

        this->font_manager = std::make_shared<FontManager>();
        this->texture_manager = std::make_shared<TextureManager>();
        this->window = std::make_shared<sf::RenderWindow>(sf::VideoMode( constants::window::size.x, constants::window::size.y ), title);
        this->effect_manager = std::make_unique<EffectManager>();

        itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);
    }

    int State::run(const std::string& path_to_pictures, const std::string& extension, const std::string& path_to_data)
    {
        return
                this->load_textures(path_to_data)
                ||this->generate_images(path_to_pictures, extension)
                ? constants::system::error_code : constants::system::pass_code;
    }

    bool State::load_fonts(const std::string& path_to_data) noexcept
    {
        return this->font_manager->load_data(path_to_data);
    }

    bool State::load_textures(const std::string& path_to_data) noexcept
    {
        return this->texture_manager->load_data(path_to_data);
    }

    bool State::generate_images(const std::string& dir, const std::string& extension)
    {
        std::vector<cv::String> fn;
        std::vector<std::string> paths;
        glob(dir + "/*" + extension, fn, false);
        paths.resize(fn.size());

        auto convert_cvstr_to_str = [](cv::String str)
                {
                    return str.operator std::string();
                };

        std::transform(fn.begin(), fn.end(), paths.begin(), convert_cvstr_to_str);

        std::string output = dir + "../output";

        for(int i = 0; i < this->texture_manager->size(); i++)
        {
            this->background.setTexture(this->texture_manager->get(i));
            for(auto& var: paths)
            {
                this->process_line(var, output);
            }
        }
    }

    bool State::process_line(const std::string& path_to_raw, const std::string& dir_to_save) noexcept
    {
        sf::Texture sprite_texture;

        if(!sprite_texture.loadFromFile(path_to_raw))
        {
            Logger::Log(constants::texture::failed_load_texture, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
            return false;
        }

        sf::Sprite base;
        base.setTexture(sprite_texture);

        auto sprites = this->effect_manager->generateSprites(base);

        for(auto& spr : sprites)
        {
            this->window->clear();
            this->window->draw(this->background);
            this->window->draw(spr);
            this->window->display();
            sf::Texture ss_texture;
            ss_texture.create(constants::window::size.x, constants::window::size.y);
            ss_texture.update(*this->window);
            sf::Image screen = ss_texture.copyToImage();
            screen.saveToFile(dir_to_save+"/temp.png");
        }

        return true;
    }
}