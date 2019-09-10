#pragma once

#include <vector>
#include <string>

#include <boost/filesystem.hpp>

#include "../config/config.hpp"
#include "../logger/logger.hpp"

namespace itl
{
    class PerlinNoise
    {
    public:
        PerlinNoise(const std::shared_ptr<Logger>& log);
        using AlfaMap = std::vector<std::vector<float>>;

    private:
        void generate(const std::string& dir) const noexcept;
        void save_to_file(const AlfaMap& map, const std::string& dir) const;
        void read_from_file(const std::string& dir);

        std::vector<AlfaMap> noises;
        std::shared_ptr<Logger> logger;
    };
}