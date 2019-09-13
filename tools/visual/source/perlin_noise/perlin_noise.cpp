#include "perlin_noise.hpp"

namespace itl
{
    PerlinNoise::PerlinNoise(const std::shared_ptr<Logger> &log, const std::string& path_to_data)
            : logger(log) {
        this->logger->log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                          Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

        for (int i = 0; i < constants::perlin::noises_num; i++)
        {
            std::stringstream full_path_perlin;
            full_path_perlin << path_to_data
                             << "/noise_"
                             << std::to_string(i)
                             << ".in";

            if(!boost::filesystem::exists(full_path_perlin.str()))
            {
                std::stringstream missing_message;
                missing_message << constants::perlin::missing_noise
                                << std::to_string(i);

                this->logger->log(missing_message.str(), Logger::STREAM::CONSOLE, Logger::TYPE::INFO);
                this->generate_noise(full_path_perlin.str(), i);
            }else
            {
                this->read_from_file(full_path_perlin.str());
            }
        }

        this->logger->log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                          Logger::STREAM::CONSOLE, Logger::TYPE::INFO);
    }

    void PerlinNoise::generate_noise(const std::string& dir, int idx) const
    {
        auto noise = new float[constants::perlin::size * constants::perlin::size * sizeof(float)];
        std::array<int, constants::perlin::size> permutation{};
        std::iota(permutation.begin(), permutation.end(), 0);
        std::random_device rand;
        std::mt19937 machine(rand());
        std::shuffle(permutation.begin(), permutation.end(), machine);

        for(int y = 0; y < constants::perlin::size; y++)
        {
            for(int x = 0; x < constants::perlin::size; x++)
            {
                noise[y * constants::perlin::size + x] = generate_point_noise(x, y, 0, permutation);
            }
        }

    }

    void PerlinNoise::save_to_file(const cv::Mat &map, const std::string &dir) const
    {
        std::ofstream output(dir);

    }

    void PerlinNoise::read_from_file(const std::string &dir)
    {
        //todo
    }

    float** PerlinNoise::get_random_noise() const
    {
        return &(this->noises[Math::random(0, static_cast<int>(this->noises.size()))]);
    }


    float PerlinNoise::generate_point_noise(int x, int y, int z,
                                           const std::array<int, constants::perlin::size> &permutation) const
    {

    }
}