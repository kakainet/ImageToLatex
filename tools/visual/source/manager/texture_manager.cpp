#include "texture_manager.hpp"

namespace itl
{

    TextureManager::TextureManager()
    {
        Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

        this->data_paths =
                {
                        "/textures/white.png"
                };

        Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE, Logger::TYPE::INFO);
    }

    bool TextureManager::load_data(const std::string& path_to_data, int number_of_copies)
    {
        this->path_to_data = path_to_data;

        if(this->data_paths.empty())
        {
            Logger::Log(constants::manager::data_path_empty, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
            return false;
        }

        bool failed = false;

        auto data_to_load = path_to_data + this->data_paths[0];

        while(number_of_copies--)
        {
            sf::Texture next_texture;
            if(!next_texture.loadFromFile(data_to_load))
            {
                failed = true;
                Logger::Log(constants::system::not_found, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
                break;
            }

            this->storage.emplace_back(std::pair<std::string, sf::Texture>({data_to_load, next_texture}));
        }

        return !failed;
    }

    bool TextureManager::update_single(const std::string& path, int idx_copy)
    {
        std::lock_guard<std::mutex> lck(mtx);

        if(!this->storage[idx_copy].second.loadFromFile(path_to_data + path))
        {
            return false;
        }

        this->storage[idx_copy].first = path;

        return true;
    }
}
