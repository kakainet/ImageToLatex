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

        explicit Logger(bool all, bool info, bool suggestions, bool errors, bool warnings);
        explicit Logger();

        void log(const std::string&,
                const Logger::STREAM& = Logger::STREAM::CONSOLE,
                const Logger::TYPE& = Logger::TYPE::ERROR) noexcept;

        void init(bool all, bool info, bool suggestions, bool errors, bool warnings);

    private:

        void sendMessage(const std::string& message,
                Logger::STREAM stream,
                std::string& prefix_colored) noexcept;

        void consoleMessage(const std::string&message,
                std::string& prefix,
                std::time_t& time) noexcept;

        void fileMessage(const std::string&,
                std::string& prefix,
                std::time_t& time) noexcept;

        void setPrefix(Logger::TYPE type,
                std::string& prefix) noexcept;

        void getColorByType() noexcept;

        bool log_all;

        bool log_info;
        bool log_suggestions;
        bool log_errors;
        bool log_warnings;
    };
}