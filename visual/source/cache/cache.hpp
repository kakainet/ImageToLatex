#pragma once

#include <vector>
#include <map>
#include <memory>
#include <type_traits>

#include "../../external/sfml/include/SFML/Graphics/Font.hpp"
#include "../logger/logger.hpp"
#include "../config/config.hpp"

namespace itl
{
	/**
	* @brief basic container of data
	*/
    template <typename T, typename std::enable_if<std::is_base_of<sf::Font, T>::value>::type>
    class Cache
	{
		/* ===Objects=== */
	private:

		//main struct for keep data
		std::map<std::string, std::shared_ptr<T>> resources;
		/* ===Methods=== */
	public:
		/**
		* @brief gets currently loaded data or loads it to memory
		* @param path to data
		  @return shared pointer to data
		*/
		std::shared_ptr<T> get(const std::string& path)
        {
            if (path.empty())
            {
                Logger::Log(constants::cache::empty_path, Logger::STREAM::BOTH, Logger::TYPE::WARNING);
            }

            {//Try find resource
                auto result = resources.find(path);
                if (result != resources.end())
                    return result->second;
            }

            {//Try to load it
                T resource;

                if (!resource.loadFromFile(path))
                {
                    Logger::Log(constants::cache::not_found, Logger::STREAM::BOTH, Logger::TYPE::ERROR);
                }

                resources.emplace(path, std::make_shared<T>(resource));

                return resources[path];
            }
        }
	};
}