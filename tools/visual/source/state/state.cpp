#include "state.hpp"

namespace itl
{
    State::State(const std::string& title)
        :hardware_concurrency(std::thread::hardware_concurrency())
    {
        itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

        this->texture_manager = std::make_shared<TextureManager>();
        for(int i = 0; i < this->hardware_concurrency; i++)
        {
            this->windows.emplace(std::make_shared<sf::RenderWindow>(sf::VideoMode( constants::window::size.x, constants::window::size.y ), title));

        }
        this->effect_manager = std::make_unique<EffectManager>();
        this->thread_pool = std::make_unique<ThreadPool>(this->hardware_concurrency);

        itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

        this->assigned_threads_to_data = 0;
    }

    int State::run(const std::string& path_to_pictures, const std::string& extension, const std::string& path_to_data)
    {
        return
                this->load_textures(path_to_data)
                && this->generate_images(path_to_pictures, extension)
                ? constants::system::pass_code : constants::system::error_code;
    }

    bool State::load_textures(const std::string& path_to_data) noexcept
    {
        return this->texture_manager->load_data(path_to_data, this->hardware_concurrency);
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

        std::vector<std::future<bool>> results;
        for(int i = 0; i < this->texture_manager->size(); i++)
        {
            for(auto& var: paths)
            {
                results.emplace_back(this->thread_pool->enqueue(&State::process_line, this, var, output, i, extension));
            }
        }

        return std::accumulate(results.begin(), results.end(), true,
                [=](bool acc, auto&& res) { return acc && res.get(); });
    }

    bool State::process_line(const std::string path_to_raw, const std::string dir_to_save,
            int background_number, const std::string extension) noexcept
    {
        if(this->assigned_threads_to_data != this->hardware_concurrency &&
           this->convert_from_thread_to_texture.find(std::this_thread::get_id()) !=
           this->convert_from_thread_to_texture.end())
        {
            this->convert_from_thread_to_texture[std::this_thread::get_id()] = assigned_threads_to_data++;
        }

        std::lock_guard<std::mutex> lck(mutex);
        std::thread::id this_id = std::this_thread::get_id();
        auto window_guard = this->windows.front();
        this->windows.pop();
        auto background_guard = this->texture_manager->get(
                this->convert_from_thread_to_texture[std::this_thread::get_id()], background_number);
        int itr = 0;
        sf::Texture sprite_texture;

        if(!sprite_texture.loadFromFile(path_to_raw))
        {
            Logger::Log(constants::texture::failed_load_texture, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
            return false;
        }


        sf::Sprite base;
        sf::Sprite background;
        background.setTexture(background_guard);
        base.setTexture(sprite_texture);
        auto file_name = (path_to_raw.substr(path_to_raw.find_last_of('/')+1));
        file_name = file_name.substr(0, file_name.find_last_of('.'));
        auto sprites = this->effect_manager->generateSprites(base);

        for(auto& spr : sprites)
        {
            window_guard.get()->clear();
            window_guard.get()->draw(background);
            window_guard.get()->draw(*spr);
            sf::Texture ss_texture;
            mutex.lock();
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
            mutex.unlock();
            screen.saveToFile(path_to_save.str());
        }

        this->windows.push(window_guard);
        return true;
    }
}