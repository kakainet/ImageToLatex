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
                && this->generate_images(path_to_pictures, extension)
                ? constants::system::pass_code : constants::system::error_code;
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
        glob(dir + "/input/*" + extension, fn, false);
        paths.resize(fn.size());

        auto convert_cvstr_to_str = [](cv::String str)
                {
                    return str.operator std::string();
                };

        std::transform(fn.begin(), fn.end(), paths.begin(), convert_cvstr_to_str);

        std::string output = dir + "/output";

        for(int i = 0; i < this->texture_manager->size(); i++)
        {
            auto texture = this->texture_manager->get(i);
            this->background.setTexture(texture);
            for(auto& var: paths)
            {
                if(!this->process_line(var, output, std::to_string(i), extension))
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool State::process_line(const std::string& path_to_raw, const std::string& dir_to_save,
            const std::string& background_number, const std::string& extension) noexcept
    {
        int itr = 0;
        sf::Texture sprite_texture;

        if(!sprite_texture.loadFromFile(path_to_raw))
        {
            Logger::Log(constants::texture::failed_load_texture, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
            return false;
        }

        sf::Sprite base;
        base.setTexture(sprite_texture);
        auto file_name = (path_to_raw.substr(path_to_raw.find_last_of('/')+1));
        file_name = file_name.substr(0, file_name.find_last_of('.'));
        auto sprites = this->effect_manager->generateSprites(base);

        for(auto& spr : sprites)
        {
            this->window->clear();
            this->window->draw(this->background);
            this->window->draw(*spr);
            this->window->display();
            sf::Texture ss_texture;
            ss_texture.create(constants::window::size.x, constants::window::size.y);
            ss_texture.update(*this->window);
            sf::Image screen = ss_texture.copyToImage();
            std::stringstream path_to_save;
            path_to_save << dir_to_save
                         << "/"
                         << background_number
                         << "_" << file_name
                         << "_"
                         << std::to_string(itr++)
                         << extension;
            screen.saveToFile(path_to_save.str());
        }

        return true;
    }
}