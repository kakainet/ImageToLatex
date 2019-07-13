#include "ResourceGuard.hpp"

template<class T>
itl::ResourceGuard<T>::ResourceGuard(T obj)
{
    this->item = std::make_shared<T>(obj);
}

template<class T>
bool itl::ResourceGuard<T>::isUsed()
{
    return this.used;
}

template<class T>
void itl::ResourceGuard<T>::setStatus(bool use)
{
    this.used = use;
}

template<class T>
std::shared_ptr<T> ResourceGuard<T>::get()
{
    return this.item;
}
