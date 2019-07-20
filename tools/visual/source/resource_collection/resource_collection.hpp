#pragma once

#include <vector>

#include "../resource_guard/resource_guard.hpp"
#include "../logger/logger.hpp"
#include "../config/config.hpp"

namespace itl
{
    template <class T>
    class ResourceCollection
    {
    public:
        ResourceGuard<T>& getFree();
        void push(T obj);
        void push(std::shared_ptr<T> obj_ptr);
    private:
        std::vector<ResourceGuard<T>> guards;
        std::mutex mutex;
    };

    template<class T>
    ResourceGuard<T>& ResourceCollection<T>::getFree()
    {
        mutex.lock();
        for(auto& guard : guards)
        {
            if(!guard.isUsed())
            {
                guard.setStatus(true);
                mutex.unlock();
                return guard;
            }
        }
        mutex.unlock();
        itl::Logger::Log(constants::thread::fail_distr_worker, itl::Logger::STREAM::BOTH, itl::Logger::TYPE::ERROR);
        exit(1);
    }

    template<class T>
    void ResourceCollection<T>::push(T obj)
    {
        this->guards.emplace_back(obj);
    }

    template<class T>
    void ResourceCollection<T>::push(std::shared_ptr<T> obj_ptr)
    {
        this->guards.emplace_back(obj_ptr);
    }
}
