#include "perlin_noise.hpp"

namespace itl {
    PerlinNoise::PerlinNoise(const std::shared_ptr<Logger> &log)
            : logger(log) {
        this->logger->log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                          Logger::STREAM::CONSOLE, Logger::TYPE::INFO);


        for (int i = 0; i < constants::perlin_noise::noises_num; i++) {
            if (!std::filesystem::exists("../../data/perlin" + std::to_string(i) + ".in")) {
                this->generate("../../data/perlin" + std::to_string(i) + ".in");
            } else {
                this->read_from_file("../../data/perlin" + std::to_string(i) + ".in");
            }
        }

        this->logger->log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                          Logger::STREAM::CONSOLE, Logger::TYPE::INFO);
    }

    void itl::PerlinNoise::generate(const std::string &dir) const noexcept {
        //todo
    }

    void itl::PerlinNoise::save_to_file(const cv::Mat &map, const std::string &dir) const {
        std::ofstream output(dir);

    }

    void PerlinNoise::read_from_file(const std::string &dir)
    {
        //todo
    }
}