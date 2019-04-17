#pragma once

#include <vector>
#include <string>

#include <boost/filesystem.hpp>

#include "../config/config.hpp"

class PerlinNoise
{
public:
    PerlinNoise();

    using AlfaMap = std::vector<std::vector<float>>;
private:
    std::vector<AlfaMap> noises;

    void generate(const std::string& dir) const noexcept;
    void save_to_file(const AlfaMap& map, const std::string& dir) const;
    void read_from_file(const std::string& dir);
};