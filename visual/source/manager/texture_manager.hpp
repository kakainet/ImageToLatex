#pragma once

#include <SFML/Graphics/Font.hpp>
#include "manager.hpp"

namespace itl
{
    class TextureManager : public Manager<sf::Texture>
    {
    public:
        virtual bool load_data() override;
        virtual sf::Texture& get_random_element() override;
    };
}
