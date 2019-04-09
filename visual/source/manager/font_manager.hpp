#pragma once

#include <SFML/Graphics/Font.hpp>
#include "manager.hpp"

namespace itl
{
    class FontManager : public Manager<sf::Font ,std::vector<sf::Font>>
    {
    public:
        virtual bool load_data() override;
        virtual sf::Font& get_random_element() override;
    };
}
