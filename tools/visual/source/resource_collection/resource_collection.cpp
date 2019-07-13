#include "ResourceCollection.hpp"

template<class T>
itl::ResourceGuard<T>& itl::ResourceCollection<T>::getFree()
{
    for(auto& guard : guards)
    {
        if(guard.isUsed())
        {
            return guard;
        }
    }

    itl::Logger::Log(constants::system::wrong_args_size, itl::Logger::STREAM::BOTH, itl::Logger::TYPE::ERROR);
    exit(1);
}