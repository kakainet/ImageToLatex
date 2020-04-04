#include "image_manager.hpp"

namespace itl
{
    ImageManager::ImageManager()
    {
        this->imgs.reserve(cst::image::img_mgr_reserve);
    }

    bool ImageManager::append(const std::vector<std::string>& paths)
    {
        return std::accumulate(paths.begin(), paths.end(), true,
                               [this](bool acc, const std::string& str) {
                                   return acc && this->append(str);
                               });
    }

    bool ImageManager::append(const std::string& path)
    {
        cv::Mat background(cv::imread(path, cv::IMREAD_UNCHANGED));

        if(!background.data || background.rows != cst::window::dim ||
           background.cols != cst::window::dim)
        {
            std::stringstream ss;
            ss << cst::texture::failed_load_texture
               << "\n\tTexture path: " << path;
            Logger::log(ss.str(), Logger::stream_t::console,
                              Logger::type_t::error);

            return false;
        }

        this->imgs.emplace_back(background);

        return true;
    }

    cv::Mat& ImageManager::operator[](int idx) { return this->imgs[idx]; }
}  // namespace itl