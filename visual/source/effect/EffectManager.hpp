#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "../logger/logger.hpp"
#include "../config/config.hpp"
#include "../math/math.hpp"

#include <memory>
#include <vector>
#include <map>
#include <functional>

#include <string.h>

namespace itl
{
    class EffectManager
    {
    public:
        EffectManager();
        std::vector<sf::Sprite> generateSprites(sf::Sprite& sprite);

    private:
        std::map<std::string, std::vector<std::function<void(sf::Sprite&)>>> functions;

        /**
         * @brief packIndexes[i][j] is index of j'th effect from i'th pack.
         */
        std::vector<std::vector<int>> packIndexes;

        void load_functions();

        void generate_all_effect_packs(std::map<std::string, std::vector<std::function<void(sf::Sprite&)>>>::iterator itr,
                                       std::vector<int> currentPack);

    };
}