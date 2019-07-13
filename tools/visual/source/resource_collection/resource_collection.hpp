#pragma once

#include <vector>

#include "../resource_guard/ResourceGuard.hpp"
#include "../logger/logger.hpp"

namespace itl
{
    template <class T>
    class ResourceCollection
    {
    public:
        ResourceGuard<T>& getFree();
    private:
        std::vector<ResourceGuard<T>> guards;
    };
}
