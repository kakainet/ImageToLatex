#include "EffectManager.hpp"


itl::EffectManager::EffectManager()
{
    itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                     Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

    this->load_functions();
    this->generate_all_effect_packs(this->functions.begin(), {});

    itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
            Logger::STREAM::CONSOLE,Logger::TYPE::INFO);
}



void itl::EffectManager::load_functions()
{
    this->functions["scale"] = std::vector<std::function<void(sf::Sprite&)>>();
    this->functions["scale"].emplace_back([](sf::Sprite& sprite)
                                           {
                                               auto val = Math::random_float(
                                                       constants::effect::scale_incr_bounds.x,
                                                       constants::effect::scale_incr_bounds.y,
                                                       constants::effect::accuracy);
                                               sprite.setScale({val, val});
                                           });

    this->functions["scale"].emplace_back([](sf::Sprite& sprite)
                                          {
                                              auto rand = [](){return Math::random_float(
                                                      constants::effect::scale_incr_bounds.x,
                                                      constants::effect::scale_incr_bounds.y,
                                                      constants::effect::accuracy);};
                                              sprite.setScale({rand(), rand()});
                                          });

    this->functions["scale"].emplace_back([](sf::Sprite& sprite)
                                          {
                                              auto val = Math::random_float(
                                                      constants::effect::scale_decr_bounds.x,
                                                      constants::effect::scale_decr_bounds.y,
                                                      constants::effect::accuracy);
                                              sprite.setScale({val, val});
                                          });

    this->functions["scale"].emplace_back([](sf::Sprite& sprite)
                                          {
                                              auto rand = [](){return Math::random_float(
                                                      constants::effect::scale_decr_bounds.x,
                                                      constants::effect::scale_decr_bounds.y,
                                                      constants::effect::accuracy);};
                                              sprite.setScale({rand(), rand()});
                                          });

    this->functions["rotate"] = std::vector<std::function<void(sf::Sprite&)>>();
    this->functions["rotate"].emplace_back([](sf::Sprite& sprite)
                                           {
                                               auto val = Math::random_float(
                                                       0,
                                                       constants::effect::max_degree,
                                                       constants::effect::accuracy);
                                               sprite.setRotation(val);
                                           });

    this->functions["rotate"].emplace_back([](sf::Sprite& sprite)
                                            {
                                                auto val = Math::random_float(
                                                        -constants::effect::max_degree,
                                                        0,
                                                        constants::effect::accuracy);
                                                sprite.setRotation(val);
                                            });

    this->functions["position"].emplace_back([](sf::Sprite& sprite)
                                            {
                                                auto val = sf::Vector2f(
                                                        Math::random(0, constants::window::size.x),
                                                        Math::random(0, constants::window::size.y));
                                                sprite.setPosition(val);
                                            });
}

void itl::EffectManager::generate_all_effect_packs(std::map<std::string, std::vector<std::function<void(sf::Sprite&)>>>::iterator itr,
        std::vector<int> currentPack)
{
    if(itr == this->functions.end())
    {
        this->packIndexes.emplace_back(currentPack);
        return;
    }

    for(int idx = 0; idx < itr->second.size(); idx++)
    {
        std::vector<int> copy = currentPack;
        copy.emplace_back(idx);
        this->generate_all_effect_packs(++itr--, copy);
    }
}

std::vector<sf::Sprite> itl::EffectManager::generateSprites(sf::Sprite &sprite)
{
    return std::vector<sf::Sprite>();
}