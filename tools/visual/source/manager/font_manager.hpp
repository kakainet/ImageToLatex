#pragma once

#include <SFML/Graphics/Font.hpp>
#include "manager.hpp"

namespace itl
{
    class FontManager : public Manager<sf::Font>
    {
    public:
        FontManager();
        virtual bool load_data(const std::string& path_to_data) override;
        virtual sf::Font& get_random_element() override;
    };
}
