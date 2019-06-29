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
        this->background = std::make_unique<sf::Sprite>();

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
        for(auto& var: paths)
        {
            this->process_line(var, output);
        }
    }

    bool State::process_line(const std::string& path_to_raw, const std::string& dir_to_save) noexcept
    {
        this->prepare_sprite(path_to_raw);
        window->clear();
        window->display();

        itl::Logger::Log(path_to_raw, itl::Logger::STREAM::CONSOLE, itl::Logger::TYPE ::INFO);
        return true;
    }

    void State::prepare_sprite(const std::string& path_to_raw)
    {
        sf::Texture texture;
        if (!texture.loadFromFile(path_to_raw))
        {
            itl::Logger::Log(constants::texture::failed_load_texture ,itl::Logger::STREAM::BOTH, itl::Logger::TYPE::ERROR);
        }

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setPosition({static_cast<float>(constants::window::size.x)/2,
                            static_cast<float>(constants::window::size.y)/2});
    }
}