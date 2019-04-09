#pragma once

#include <memory>

#include "../logger/logger.hpp"
#include "../config/config.hpp"
#include "../math/math.hpp"

template <class T, class C>
class Manager
{
protected:
    std::unique_ptr<C> storage;
public:
    Manager();
    virtual bool load_data() = 0;
    virtual T& get_random_element() = 0;
};

template<class T, class C>
Manager<T, C>::Manager()
{
    this->storage = std::make_unique<C>();
}
