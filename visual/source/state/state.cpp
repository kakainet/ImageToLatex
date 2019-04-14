#include "state.hpp"

namespace itl
{
    State::State()
    {
        this->font_manager = std::make_shared<FontManager>();
        this->texture_manager = std::make_shared<TextureManager>();
    }

    int State::run()
    {
        return
                this->load_fonts()
                ||this->load_textures()
                ||this->generate_latex_seq()
                ||this->generate_images()
                ? 1 : 0;
    }

    bool State::load_fonts()
    {
        return this->font_manager->load_data();
    }

    bool State::load_textures()
    {
        return this->texture_manager->load_data();
    }

    bool State::generate_latex_seq()
    {
        return static_cast<bool>(system(constants::system::latex_path));
    }

    bool State::generate_images()
    {
        return false;
    }
}

