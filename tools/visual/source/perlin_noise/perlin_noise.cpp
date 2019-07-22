#include "perlin_noise.hpp"

itl::PerlinNoise::PerlinNoise()
{
    itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                     Logger::STREAM::CONSOLE, Logger::TYPE::INFO);


    for(int i = 0; i < constants::perlin_noise::noises_num; i++)
    {
        if(!boost::filesystem::exists("../../data/perlin"+std::to_string(i)+".in"))
        {
            this->generate("../../data/perlin"+std::to_string(i)+".in");
        }
        else
        {
            this->read_from_file("../../data/perlin"+std::to_string(i)+".in");
        }
    }

    itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                     Logger::STREAM::CONSOLE, Logger::TYPE::INFO);
}

void itl::PerlinNoise::generate(const std::string &dir) const noexcept
{
    //todo
}

void itl::PerlinNoise::save_to_file(const PerlinNoise::AlfaMap &map, const std::string &dir) const
{
    std::ofstream output(dir);
    output<<map.begin()->size()<<' '<<map.size()<<'\n';
    for(int i = 0; i<map.size(); i++)
    {
        for(int j = 0; j<map.size(); j++)
        {
            j == map.size()-1   ?
                                output<<map[i][j]<<' ' :
                                output<<map[i][j];
        }
        output<<'\n';
    }
}

void itl::PerlinNoise::read_from_file(const std::string& dir)
{
    noises.emplace_back();
    std::ifstream input(dir);
    int32_t x,y;
    input>>x>>y;
    for(int j = 0; j < y; j++)
    {
        noises.back().emplace_back();
        for(int i = 0; i < x; i++)
        {
            float temp = 0;
            input>>temp;
            noises.back().back().emplace_back(temp);
        }
    }
}
