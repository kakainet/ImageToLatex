#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>

#include "../config/config.hpp"

namespace itl
{

    class Logger
    {
    public:

        enum class STREAM
        {
            CONSOLE,
            FILE,
            BOTH
        };

        enum class TYPE
        {
            INFO,
            SUGGESTION ,
            WARNING,
            ERROR
        };

        void log(const std::string&,
                const Logger::STREAM& = Logger::STREAM::CONSOLE,
                const Logger::TYPE& = Logger::TYPE::ERROR) noexcept;
    private:


        void sendMessage(const std::string& message,
                Logger::STREAM stream,
                std::string& prefix) noexcept;

        void consoleMessage(const std::string&message,
                std::string& prefix,
                std::time_t& time) noexcept;

        void fileMessage(const std::string&,
                std::string& prefix,
                std::time_t& time) noexcept;

        void setPrefix(Logger::TYPE type,
                std::string& prefix) noexcept;
    };
}