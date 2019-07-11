#include "EffectManager.hpp"


itl::EffectManager::EffectManager()
{
    itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                     Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

    this->load_functions();
    this->generate_all_effect_packs();

    itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
            Logger::STREAM::CONSOLE,Logger::TYPE::INFO);
}



void itl::EffectManager::load_functions()
{
    this->functions[FUNCTION_T::SCALE] = std::vector<std::function<void(sf::Sprite&)>>();
    this->functions[FUNCTION_T::SCALE].emplace_back([](sf::Sprite& sprite)
                                           {
                                               auto val = Math::random_float(
                                                       constants::effect::scale_incr_bounds.x,
                                                       constants::effect::scale_incr_bounds.y,
                                                       constants::effect::accuracy);
                                               sprite.setScale({val, val});
                                           });

    this->functions[FUNCTION_T::SCALE].emplace_back([](sf::Sprite& sprite)
                                          {
                                              auto rand = [](){return Math::random_float(
                                                      constants::effect::scale_incr_bounds.x,
                                                      constants::effect::scale_incr_bounds.y,
                                                      constants::effect::accuracy);};
                                              sprite.setScale({rand(), rand()});
                                          });

    this->functions[FUNCTION_T::SCALE].emplace_back([](sf::Sprite& sprite)
                                          {
                                              auto val = Math::random_float(
                                                      constants::effect::scale_decr_bounds.x,
                                                      constants::effect::scale_decr_bounds.y,
                                                      constants::effect::accuracy);
                                              sprite.setScale({val, val});
                                          });

    this->functions[FUNCTION_T::SCALE].emplace_back([](sf::Sprite& sprite)
                                          {
                                              auto rand = [](){return Math::random_float(
                                                      constants::effect::scale_decr_bounds.x,
                                                      constants::effect::scale_decr_bounds.y,
                                                      constants::effect::accuracy);};
                                              sprite.setScale({rand(), rand()});
                                          });

    this->functions[FUNCTION_T::ROTATE] = std::vector<std::function<void(sf::Sprite&)>>();
    this->functions[FUNCTION_T::ROTATE].emplace_back([](sf::Sprite& sprite)
                                           {
                                               auto val = Math::random_float(
                                                       0,
                                                       constants::effect::max_degree,
                                                       constants::effect::accuracy);
                                               sprite.setRotation(val);
                                           });

    this->functions[FUNCTION_T::ROTATE].emplace_back([](sf::Sprite& sprite)
                                            {
                                                auto val = Math::random_float(
                                                        -constants::effect::max_degree,
                                                        0,
                                                        constants::effect::accuracy);
                                                sprite.setRotation(val);
                                            });

    this->functions[FUNCTION_T::POSITION].emplace_back([](sf::Sprite& sprite)
                                            {
                                                auto val = sf::Vector2f(
                                                        Math::random(0, constants::window::size.x),
                                                        Math::random(0, constants::window::size.y));
                                                sprite.setPosition(val);
                                            });
}


void itl::EffectManager::generate_all_effect_packs()
{
    const int n = functions.size();

    int* indices = new int[n];

    for (int i = 0; i < n; i++)
    {
        indices[i] = 0;
    }

    while (true)
    {
        packIndexes.emplace_back();

        for (int i = 0; i < n; i++)
        {
            packIndexes.back().emplace_back(indices[i]);
        }

        int next = n - 1;
        while (next >= 0 && (indices[next] + 1 >= this->functions[static_cast<FUNCTION_T>(next)].size()))
        {
            next--;
        }

        if (next < 0)
        {
            return;
        }

        indices[next]++;

        for (int i = next + 1; i < n; i++)
        {
            indices[i] = 0;
        }
    }
}

std::vector<sf::Sprite> itl::EffectManager::generateSprites(sf::Sprite &sprite)
{
    auto result = std::vector<sf::Sprite>();

    for(int i = 0; i < this->packIndexes.size(); i++)
    {
        sf::Sprite new_sprite;
        for(int j = 0; j < this->packIndexes[i].size(); j++)
        {
            this->functions[static_cast<FUNCTION_T>(i)][j](new_sprite);
        }
        result.emplace_back(sprite);
    }

    return result;
}