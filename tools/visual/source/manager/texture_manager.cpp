#include "texture_manager.hpp"

namespace itl
{

    TextureManager::TextureManager()
    {
        itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

        itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);
    }

    bool TextureManager::load_data(const std::string& path_to_data)
    {
        bool failed = false;

        std::vector<std::string> textures =
                {
                        "white.png"
                };

        std::transform(textures.begin(),textures.end(), textures.begin(),
                [=](std::string str){ return path_to_data + "/textures/" + str;});


        for(auto& path : textures)
        {
            sf::Texture next_texture;
            if(!next_texture.loadFromFile(path))
            {
                failed = true;
                Logger::Log(constants::system::not_found, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
                break;
            }
            this->storage.push_back(next_texture);
        }

        return !failed;
    }
}
