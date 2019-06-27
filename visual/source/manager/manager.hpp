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
        std::unique_ptr<std::vector<T>> storage;
    public:
        Manager();
        virtual bool load_data(const std::string& path_to_data) = 0;
        virtual T& get_random_element() = 0;
    };

    template<class T>
    Manager<T>::Manager()
    {
        this->storage = std::make_unique<std::vector<T>>();
    }
}