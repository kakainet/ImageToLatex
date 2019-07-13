#pragma once

#include <memory>

namespace itl
{
    template <class T>
    class ResourceGuard
    {
    public:
        explicit ResourceGuard(T obj);
        explicit ResourceGuard(std::shared_ptr<T>& obj_ptr);
        bool isUsed();
        void setStatus(bool use);
        std::shared_ptr<T> get();
    private:
        bool used;
        std::shared_ptr<T> item;
    };

    template<class T>
    ResourceGuard<T>::ResourceGuard(T obj)
            :used(false)
    {
        this->item = std::make_shared<T>(obj);
    }

    template<class T>
    ResourceGuard<T>::ResourceGuard(std::shared_ptr<T>& obj_ptr)
            :used(false)
    {
        this->item = obj_ptr;
    }

    template<class T>
    bool ResourceGuard<T>::isUsed()
    {
        return this->used;
    }

    template<class T>
    void ResourceGuard<T>::setStatus(bool use)
    {
        this->used = use;
    }

    template<class T>
    std::shared_ptr<T> ResourceGuard<T>::get()
    {
        return this->item;
    }
}
