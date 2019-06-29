#pragma once

#include <SFML/Graphics/Sprite.hpp>

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
        std::vector<std::function<void(sf::Sprite&)>> generateEffectPack();

    private:
        std::map<std::string, std::vector<std::function<void(sf::Sprite&)>>> functions;

        void loadFunctions();
    };
}