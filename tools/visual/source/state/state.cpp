#include "state.hpp"

namespace itl
{
    State::State(const std::string& title)
    {
        itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

        this->font_manager = std::make_shared<FontManager>();
        this->texture_manager = std::make_shared<TextureManager>();
        for(int i = 0; i < std::thread::hardware_concurrency(); i++)
        {
            this->windows.push(std::make_shared<sf::RenderWindow>(sf::VideoMode( constants::window::size.x, constants::window::size.y ), title));
            this->backgrounds.push(sf::Sprite());
        }
        this->effect_manager = std::make_unique<EffectManager>();
        this->thread_pool = std::make_unique<ThreadPool>(std::thread::hardware_concurrency());

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
            for(auto& var: paths)
            {
                std::future<bool> result = this->thread_pool->enqueue(&State::process_line, this, var, output, i, extension);
                if(!result.get())
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool State::process_line(const std::string& path_to_raw, const std::string& dir_to_save,
            int background_number, const std::string& extension) noexcept
    {
        auto background_texture = this->texture_manager->get(background_number);

        auto window_guard = this->windows.getFree();
        auto background_guard = this->backgrounds.getFree();

        background_guard.get()->setTexture(background_texture);

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
            window_guard.get()->clear();
            window_guard.get()->draw(*background_guard.get());
            window_guard.get()->draw(*spr);
            sf::Texture ss_texture;
            ss_texture.create(constants::window::size.x, constants::window::size.y);
            ss_texture.update(*window_guard.get());
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

        window_guard.setStatus(false);
        background_guard.setStatus(false);

        return true;
    }
}