#include "font_manager.hpp"

namespace itl
{
    FontManager::FontManager()
    {
        itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

        itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);
    }

    bool FontManager::load_data()
    {
        const std::vector<std::string> fonts =
                {
                        "../../../data/fonts/arial.ttf"
                };

        for(auto& path : fonts)
        {
            sf::Font next_font;
            if(!next_font.loadFromFile(path))
            {
                Logger::Log(constants::system::not_found, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
                continue;
            }
            this->storage->emplace_back(next_font);
        }
    }

    sf::Font& FontManager::get_random_element()
    {
        return this->storage->at(static_cast<ulong>(Math::random(0,static_cast<int>(this->storage->size()))));
    }
}
