#include "texture_manager.hpp"

namespace itl
{

    TextureManager::TextureManager()
    {
        itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

        this->data_paths =
                {
                        "/textures/white.png"
                };

        itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE, Logger::TYPE::INFO);
    }

    bool TextureManager::load_data(const std::string& path_to_init_data, int number_of_copies)
    {

        bool failed = false;


        if(std::find(this->data_paths.begin(), this->data_paths.end(), path_to_init_data) == this->data_paths.end())
        {
            Logger::Log(constants::system::not_found, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
            return failed;
        }

        while(number_of_copies--)
        {
            sf::Texture next_texture;
            if(!next_texture.loadFromFile(path_to_init_data))
            {
                failed = true;
                Logger::Log(constants::system::not_found, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
                break;
            }

            this->storage.emplace_back(std::pair<std::string, sf::Texture>({path_to_init_data, next_texture}));
        }

        return !failed;
    }

    bool TextureManager::update_single(const std::string& path, int idx_copy)
    {
        if(!this->storage[idx_copy].second.loadFromFile(path))
        {
            return false;
        }

        this->storage[idx_copy].first = path;

        return true;
    }
}
