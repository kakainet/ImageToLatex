#pragma once
#include <memory>

namespace itl
{
    template <class T>
    class ResourceGuard
    {
    public:
        explicit ResourceGuard(T obj);
        bool isUsed();
        void setStatus(bool use);
        std::shared_ptr<T> get();
    private:
        bool used;
        std::shared_ptr<T> item;
    };
}
