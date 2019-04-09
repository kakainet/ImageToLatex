#include <map>
#include <vector>
#include <memory>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../config/config.hpp"

#pragma once

namespace itl
{
    class System
    {
        std::shared_ptr<std::vector<sf::Font>> fonts;
        std::shared_ptr<std::map<std::string,sf::Texture>> texture_atlas;

    public:
        /**
         * @brief runs main state of program
         * @return 0 if program finished without any problems 1 otherwise
         */
        int run();

    private:
        bool load_fonts();
        bool load_textures();
        bool generate_latex_seq();
        bool generate_images();
    };
}

