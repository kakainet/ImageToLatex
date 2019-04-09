#include "system.hpp"

namespace itl
{
    int System::run()
    {
        return
                this->load_fonts()
                ||this->load_textures()
                ||this->generate_latex_seq()
                ||this->generate_images()
                ? 1 : 0;
    }

    bool System::load_fonts()
    {
        return false;
    }

    bool System::load_textures()
    {
        return false;
    }

    bool System::generate_latex_seq()
    {
        return static_cast<bool>(system(constants::system::latex_path));
    }

    bool System::generate_images()
    {
        return false;
    }
}

