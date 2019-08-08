#pragma once

#include <mutex>

#include <SFML/Graphics/Font.hpp>

#include "manager.hpp"

namespace itl
{
    class TextureManager : public Manager<sf::Texture>
    {
    public:
        TextureManager(const std::shared_ptr<Logger>& log);
        virtual bool load_data(const std::string& path_to_data, int number_of_copies) override;
        virtual bool update_single(const std::string& path, int idx_copy) override;
    private:
        std::mutex mtx;
    };
}
