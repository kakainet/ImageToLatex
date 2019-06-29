#include "EffectManager.hpp"


itl::EffectManager::EffectManager()
{
    itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                     Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

    this->loadFunctions();

    itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
            Logger::STREAM::CONSOLE,Logger::TYPE::INFO);
}



void itl::EffectManager::loadFunctions()
{
    this->functions["scale"] = std::vector<std::function<void(sf::Sprite&)>>();
    this->functions["scale"].emplace_back( [](sf::Sprite& sprite)
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
    this->functions["rotate"].emplace_back( [](sf::Sprite& sprite)
                                           {
                                               auto val = Math::random_float(
                                                       0,
                                                       constants::effect::max_degree,
                                                       constants::effect::accuracy);
                                               sprite.setRotation(val);
                                           });

    this->functions["rotate"].emplace_back( [](sf::Sprite& sprite)
                                            {
                                                auto val = Math::random_float(
                                                        -constants::effect::max_degree,
                                                        0,
                                                        constants::effect::accuracy);
                                                sprite.setRotation(val);
                                            });
}

std::vector<std::function<void(sf::Sprite &)>> itl::EffectManager::generateEffectPack()
{
    auto vec = std::vector<std::function<void(sf::Sprite &)>>();

    for(auto& var: this->functions)
    {
       // vec.emplace_back(var.second)
    }
}
