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
        std::vector<std::string> data_paths;
        Manager();
        using path_t = std::string;
        std::vector<std::pair<path_t, T>> storage;
    public:
        virtual bool load_data(const std::string& path_to_data, int number_of_copies) = 0;
        virtual bool update_single(const std::string& path_to_data, int idx_copy) = 0;
        int size() const;
        T& get(int idx_copy, int idx_data);
    };

    template<class T>
    int Manager<T>::size() const
    {
        this->storage.size();
    }

    template<class T>
    T& Manager<T>::get(int idx_copy, int idx_data)
    {
        if(this->storage[idx_copy].first != this->data_paths[idx_data])
        {
            if(!this->update_single(this->data_paths[idx_data], idx_copy))
            {
                Logger::Log(constants::manager::failed_update, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
            }
        }

        return this->storage[idx_copy].second;
    }

    template<class T>
    Manager<T>::Manager()
    {
        itl::Logger::Log(std::string(constants::info::init_module_msg_start) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE, Logger::TYPE::INFO);

        itl::Logger::Log(std::string(constants::info::init_module_msg_end) + std::string(typeid(this).name()),
                         Logger::STREAM::CONSOLE, Logger::TYPE::INFO);
    }
}