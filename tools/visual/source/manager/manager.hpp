#pragma once

#include <memory>

#include "../logger/logger.hpp"
#include "../config/config.hpp"
#include "../math/math.hpp"

namespace itl
{
    template <class T>
    class Manager
    {
    protected:
        Manager();
        using path_t = std::string;
        std::vector<std::pair<path_t, T>> storage;
    public:
        virtual bool load_data(const std::string& path_to_data, int number_of_copies) = 0;
        int size();
        T& get(int idx, const std::string& path_to_data);
    };

    template<class T>
    int Manager<T>::size()
    {
        this->storage.size();
    }

    template<class T>
    T& Manager<T>::get(int idx, const std::string& path_to_data)
    {
        return this->storage[idx];
    }

    template<class T>
    Manager<T>::Manager()
    {
        itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);

        itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE,Logger::TYPE::INFO);
    }
}