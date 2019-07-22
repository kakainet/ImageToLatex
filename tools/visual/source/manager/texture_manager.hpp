#pragma once

#include <mutex>

#include <SFML/Graphics/Font.hpp>

#include "manager.hpp"

namespace itl
{
    class TextureManager : public Manager<sf::Texture>
    {
    public:
        TextureManager();
        virtual bool load_data(const std::string& path_to_data, int number_of_copies) override;
        virtual bool update_single(const std::string& path, int idx_copy) override;
        std::mutex mtx;
    };
}
