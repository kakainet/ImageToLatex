#include "image_manager.hpp"

explicit ImageManager::ImageManager(const std::string& path)
{
    this->logger->log(std::string(cst::info::init_module_msg_start) +
                      std::string(typeid(this).name()),
                      Logger::stream_t::console, Logger::type_t::info);

    this->imgs.reserve(cst::image::img_mgr_reserve);

    this->logger->log(std::string(cst::info::init_module_msg_end) +
                      std::string(typeid(this).name()),
                      Logger::stream_t::console, Logger::type_t::info);
}
bool append(const std::vector<std::string>& paths)
{
    std::for_each(paths.begin(), paths.end(), [this](const auto& str) {
        this->append(str);
    });
}

bool ImageManager::append(const std::string& path)
{
    cv::Mat background(
            cv::imread(path_to_background, cv::IMREAD_UNCHANGED));

    if(!background.data ||
       background.rows != cst::window::dim ||
       background.cols != cst::window::dim)
    {
        std::scoped_lock<std::mutex> lck(this->mtx);
        std::stringstream ss;
        ss << cst::texture::failed_load_texture
           << "\n\tBackground texture path: " << path_to_background;
        this->logger->log(ss.str(), Logger::stream_t::console,
                          Logger::type_t::error);

        return false;
    }
}
