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
                this->generate_noise_2d(full_path_perlin.str());
            }else
            {
                this->read_from_file(full_path_perlin.str());
            }
        }

        this->logger->log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                          Logger::STREAM::CONSOLE, Logger::TYPE::INFO);
    }

    void PerlinNoise::generate_noise_2d(const std::string& dir)
    {
        std::array<float, constants::perlin::size> noise{};
        std::array<int, constants::perlin::size> permutation{};
        std::iota(permutation.begin(), permutation.end(), 0);
        std::random_device rand;
        std::mt19937 machine(rand());
        std::shuffle(permutation.begin(), permutation.end(), machine);

        for(int y = 0; y < constants::perlin::size; y++)
        {
            for(int x = 0; x < constants::perlin::size; x++)
            {
                noise[y * constants::perlin::size + x] = generate_point_noise(
                        static_cast<float>(x) / constants::perlin::frequency,
                        static_cast<float>(y) / constants::perlin::frequency,
                        constants::perlin::size / 2.f,
                        permutation);
            }
        }

        noises.emplace_back(noise);
        save_to_file(noise, dir);
    }

    void PerlinNoise::save_to_file(const std::array<float, constants::perlin::size> &noise, const std::string &dir) const
    {
        std::ofstream output(dir);
        for(size_t i = 0; i < constants::perlin::size - 1; i++)
        {
            output << noise[i]
                   << ' ';
        }
        output << noise[constants::perlin::size - 1]
               << '\n';
    }

    void PerlinNoise::read_from_file(const std::string &dir)
    {
        //todo
    }

    const std::array<float, constants::perlin::size>& PerlinNoise::get_random_noise() const
    {
        return this->noises[Math::random(0, static_cast<int>(this->noises.size()))];
    }


    float PerlinNoise::generate_point_noise(float x, float y, float z,
                                           const std::array<int, constants::perlin::size> &permutation) const
    {
        int X = static_cast<int>(std::floor(x)) & 255,
            Y = static_cast<int>(std::floor(y)) & 255,
            Z = static_cast<int>(std::floor(z)) & 255;
        x -= static_cast<int>(std::floor(x));
        y -= static_cast<int>(std::floor(y));
        z -= static_cast<int>(std::floor(z));
        float u = Math::fade(x),
              v = Math::fade(y),
              w = Math::fade(z);
        int A = permutation[X  ]+Y, AA = permutation[A]+Z, AB = permutation[A+1]+Z,
            B = permutation[X+1]+Y, BA = permutation[B]+Z, BB = permutation[B+1]+Z;

        return Math::lerp(w,
                          Math::lerp(v, Math::lerp(u,
                                                   Math::grad(permutation[AA  ], x  , y  , z   ),
                                                   Math::grad(permutation[BA  ], x-1, y  , z   )),
                                           Math::lerp(u,
                                                      Math::grad(permutation[AB  ], x  , y-1, z   ),
                                                      Math::grad(permutation[BB  ], x-1, y-1, z   ))),
                             Math::lerp(v,
                                        Math::lerp(u,
                                                   Math::grad(permutation[AA+1], x  , y  , z-1 ),
                                                   Math::grad(permutation[BA+1], x-1, y  , z-1 )),
                                        Math::lerp(u,
                                                   Math::grad(permutation[AB+1], x  , y-1, z-1 ),
                                                   Math::grad(permutation[BB+1], x-1, y-1, z-1 ))));
    }
}