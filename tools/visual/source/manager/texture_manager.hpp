#pragma once

#include <SFML/Graphics/Font.hpp>
#include "manager.hpp"

namespace itl
{
    class TextureManager : public Manager<sf::Texture>
    {
    public:
        TextureManager();
        virtual bool load_data(const std::string& path_to_data) override;
    };
}
