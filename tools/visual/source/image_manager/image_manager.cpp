#include "image_manager.hpp"

namespace itl
{
    ImageManager::ImageManager()
    {
        this->logger = std::make_shared<Logger>();
        this->logger->log(std::string(cst::info::init_module_msg_start) +
                              std::string(typeid(this).name()),
                          Logger::stream_t::console, Logger::type_t::info);

        this->imgs.reserve(cst::image::img_mgr_reserve);
        this->logger->log(std::string(cst::info::init_module_msg_end) +
                              std::string(typeid(this).name()),
                          Logger::stream_t::console, Logger::type_t::info);
    }

    bool ImageManager::append(const std::vector<std::string>& paths)
    {
        return std::accumulate(paths.begin(), paths.end(), false,
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
            this->logger->log(ss.str(), Logger::stream_t::console,
                              Logger::type_t::error);

            return false;
        }

        this->imgs.emplace_back(background);

        return true;
    }

    cv::Mat& ImageManager::operator[](int idx) { return this->imgs[idx]; }
}  // namespace itl