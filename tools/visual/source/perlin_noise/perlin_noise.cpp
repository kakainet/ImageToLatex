#include "perlin_noise.hpp"

namespace itl
{
    PerlinNoise::PerlinNoise(const std::shared_ptr<Logger> &log, const std::string& path_to_data)
        :logger(log)
    {

        this->logger->log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                          Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

        for (int i = 0; i < constants::perlin::noises_num; i++)
        {
            std::stringstream full_path_perlin;
            full_path_perlin << path_to_data
                             << "/noises/noise_"
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
        std::vector<double> noise;
        std::vector<int> permutation(static_cast<unsigned long>(constants::perlin::permutation_size));
        std::iota(permutation.begin(), permutation.end(), 0);
        std::shuffle(permutation.begin(), permutation.end(),  std::mt19937{std::random_device{}()});
        permutation.insert(permutation.end(), permutation.begin(), permutation.end());

        for(int y = 0; y < constants::perlin::dim; y++)
        {
            for(int x = 0; x < constants::perlin::dim; x++)
            {
                std::cout<<" x="<<x<<" y="<<y<<"\n";
                noise.emplace_back(generate_point_noise(
                        static_cast<float>(x) / constants::perlin::frequency,
                        static_cast<float>(y) / constants::perlin::frequency,
                        0.f,
                        permutation));
            }
        }

        noises.emplace_back(noise);
        save_to_file(noise, dir);
    }

    void PerlinNoise::save_to_file(const std::vector<double> &noise, const std::string &dir) const
    {
        std::ofstream output(dir);

        for (size_t i = 0; i < constants::perlin::size - 1; i++) {
            output << noise[i];

            if(i != 0 && (i+1) % constants::perlin::dim == 0)
            {
                output << '\n';
            } else
            {
                output<< ' ';
            }

        }
        output << noise[constants::perlin::size - 1]
               << '\n';

    }

    void PerlinNoise::read_from_file(const std::string &dir)
    {
        //todo
    }

    const std::vector<double>& PerlinNoise::get_random_noise() const
    {
        return this->noises[Math::random(0, static_cast<int>(this->noises.size()))];
    }
    
    double PerlinNoise::generate_point_noise(double x, double y, double z,
                                           const std::vector<int> &p) const
    {
        const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & 255;
        const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & 255;
        const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & 255;

        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        const double u = Math::fade(x);
        const double v = Math::fade(y);
        const double w = Math::fade(z);

        const std::int32_t A = p[X] + Y;
        const std::int32_t AA = p[A] + Z;
        const std::int32_t AB = p[A + 1] + Z;
        const std::int32_t B = p[X + 1] + Y;
        const std::int32_t BA = p[B] + Z;
        const std::int32_t BB = p[B + 1] + Z;

        return Math::lerp(w, Math::lerp(v, Math::lerp(u, Math::grad(p[AA], x, y, z),
                                    Math::grad(p[BA], x - 1, y, z)),
                            Math::lerp(u, Math::grad(p[AB], x, y - 1, z),
                                 Math::grad(p[BB], x - 1, y - 1, z))),
                    Math::lerp(v, Math::lerp(u, Math::grad(p[AA + 1], x, y, z - 1),
                                 Math::grad(p[BA + 1], x - 1, y, z - 1)),
                         Math::lerp(u, Math::grad(p[AB + 1], x, y - 1, z - 1),
                              Math::grad(p[BB + 1], x - 1, y - 1, z - 1))));
    }
}