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

void itl::EffectManager::loadFunctions()
{

}

std::vector<std::function<void(sf::Sprite &)>> itl::EffectManager::getFunctions(const std::string &name)
{
    return std::vector<std::function<void(sf::Sprite &)>>();
}
