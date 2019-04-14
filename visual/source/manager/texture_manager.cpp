#include "texture_manager.hpp"

namespace itl
{
    bool TextureManager::load_data()
    {
        const std::vector<std::string> textures =
                {
                        "../../../data/fonts/white.png"
                };

        for(auto& path : textures)
        {
            sf::Font next_font;
            if(!next_font.loadFromFile(path))
            {
                Logger::Log(constants::system::not_found, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
                continue;
            }
            this->storage->emplace_back(next_font);
        }
    }

    sf::Texture& TextureManager::get_random_element()
    {
        return this->storage->at(static_cast<ulong>(Math::random(0,static_cast<int>(this->storage->size()))));
    }
}
