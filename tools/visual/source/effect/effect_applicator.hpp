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
    class EffectApplicator
    {
    public:
        explicit EffectApplicator(const std::shared_ptr<Logger>& log);
        std::vector<std::shared_ptr<sf::Sprite>> generateSprites(sf::Sprite& sprite);

    private:

        enum class FUNCTION_T
        {
            SCALE = 0,
            ROTATE,
            POSITION
        };

        std::map<FUNCTION_T, std::vector<std::function<void(sf::Sprite&)>>> functions;

        /**
         * @brief packIndexes[i][j] is index of j'th effect from i'th pack.
         */
        std::vector<std::vector<int>> packIndexes;

        void load_functions();

        void generate_all_effect_packs();

        std::shared_ptr<Logger> logger;

    };
}