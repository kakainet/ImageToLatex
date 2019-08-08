#include "state.hpp"

namespace itl
{
    State::State(const std::string& title, const std::shared_ptr<Logger>& log,
                 const std::shared_ptr<itl::FlagManager>& flag_manager)
        : hardware_concurrency(std::thread::hardware_concurrency()), logger(log), flag_manager(flag_manager)
    {
        this->logger->log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

        this->texture_manager = std::make_shared<TextureManager>(this->logger);
        for(int i = 0; i < this->hardware_concurrency; i++)
        {
            this->windows.emplace(std::make_shared<sf::RenderWindow>(sf::VideoMode( constants::window::size.x, constants::window::size.y ), title));

        }
        this->effect_manager = std::make_unique<EffectApplicator>(this->logger);
        this->thread_pool = std::make_unique<ThreadPool>(this->hardware_concurrency);

        this->logger->log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

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
        for(int i = 0; i < this->texture_manager->unique_size(); i++)
        {
            for(auto& var: paths)
            {
                std::string cp_output{output};
                int cp_itr{i};
                std::string cp_ext{extension};
                results.emplace_back(this->thread_pool->enqueue(&State::process_line, this, var, cp_output, cp_itr, cp_ext));
            }
        }

        return std::accumulate(results.begin(), results.end(), true,
                [=](bool acc, auto&& res) { return acc && res.get(); });
    }

    bool State::process_line(const std::string path_to_raw, const std::string dir_to_save,
                             int background_number, const std::string extension) noexcept
    {
        std::shared_ptr<sf::RenderWindow> window_guard = nullptr;
        sf::Texture* background_guard = nullptr;

        {
            std::scoped_lock<std::mutex> lck(this->mtx);
            if(this->assigned_threads_to_data != this->hardware_concurrency &&
               this->convert_from_thread_to_texture.find(std::this_thread::get_id()) !=
               this->convert_from_thread_to_texture.end())
            {
                this->convert_from_thread_to_texture[std::this_thread::get_id()] = this->assigned_threads_to_data++;
            }

            window_guard = this->windows.front();
            this->windows.pop();

            background_guard = this->texture_manager->get(
                    this->convert_from_thread_to_texture[std::this_thread::get_id()], background_number);
        }

        int itr = 0;
        sf::Texture sprite_texture;

        if(!sprite_texture.loadFromFile(path_to_raw))
        {
            std::scoped_lock<std::mutex> lck(this->mtx);
            this->logger->log(constants::texture::failed_load_texture, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
            return false;
        }


        sf::Sprite base;
        sf::Sprite background;
        std::string file_name;
        std::vector<std::shared_ptr<sf::Sprite>> sprites;

        {
            std::scoped_lock<std::mutex> lck(this->mtx);
            background.setTexture(*background_guard);
            base.setTexture(sprite_texture);
            file_name = (path_to_raw.substr(path_to_raw.find_last_of('/')+1));
            file_name = file_name.substr(0, file_name.find_last_of('.'));
            sprites = this->effect_manager->generateSprites(base);
        }


        for(auto& spr : sprites)
        {
            window_guard->clear();
            window_guard->draw(background);
            window_guard->draw(*spr);
            sf::Texture ss_texture;

            ss_texture.create(constants::window::size.x, constants::window::size.y);

            ss_texture.update(*window_guard);
            sf::Image screen = ss_texture.copyToImage();
            std::stringstream path_to_save;
            std::scoped_lock<std::mutex> lck(this->mtx);


            path_to_save << dir_to_save
                         << "/"
                         << background_number
                         << "_" << file_name
                         << "_"
                         << std::to_string(itr++)
                         << extension;

            screen.saveToFile(path_to_save.str());
        }

        {
            std::scoped_lock<std::mutex> lck(this->mtx);
            this->windows.push(window_guard);
        }

        return true;
    }
}