#include "EffectManager.hpp"


itl::EffectManager::EffectManager()
{
    itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                     Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

    this->loadFunctions();

    itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
            Logger::STREAM::CONSOLE,Logger::TYPE::INFO);
}

std::function<void(sf::Sprite &)> itl::EffectManager::getFunction(const std::string &name)
{
    return this->functions[name];
}


std::map<std::string, std::function<void(sf::Sprite&)>> itl::EffectManager::getFunctions(const std::string &name)
{
    return this->functions;
}

void itl::EffectManager::loadFunctions()
{
    this->functions.emplace("scale_random_increase_symetric",
                            [](sf::Sprite& sprite)
                            {
                                auto val = Math::random_float(
                                        constants::effect::scale_incr_bounds.x,
                                        constants::effect::scale_incr_bounds.y,
                                        constants::effect::accuracy);
                                sprite.setScale({val, val});
                            });

    this->functions.emplace("scale_random_increase_non_symetric",
                            [](sf::Sprite& sprite)
                            {
                                auto rand = [](){return Math::random_float(
                                        constants::effect::scale_incr_bounds.x,
                                        constants::effect::scale_incr_bounds.y,
                                        constants::effect::accuracy);};
                                sprite.setScale({rand(), rand()});
                            });

    this->functions.emplace("scale_random_decrease_symetric",
                            [](sf::Sprite& sprite)
                            {
                                auto val = Math::random_float(
                                        constants::effect::scale_decr_bounds.x,
                                        constants::effect::scale_decr_bounds.y,
                                        constants::effect::accuracy);
                                sprite.setScale({val, val});
                            });

    this->functions.emplace("scale_random_decrease_non_symetric",
                            [](sf::Sprite& sprite)
                            {
                                auto rand = [](){return Math::random_float(
                                        constants::effect::scale_decr_bounds.x,
                                        constants::effect::scale_decr_bounds.y,
                                        constants::effect::accuracy);};
                                sprite.setScale({rand(), rand()});
                            });
}