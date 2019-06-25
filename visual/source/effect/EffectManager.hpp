#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "../logger/logger.hpp"
#include "../config/config.hpp"

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

        std::function<void(sf::Sprite&)> getFunction(const std::string& name);
        std::vector<std::function<void(sf::Sprite&)>> getFunctions(const std::string& name);

    private:
        std::map<std::string, std::function<void(sf::Sprite&)>> functions;

        void loadFunctions();
    };
}