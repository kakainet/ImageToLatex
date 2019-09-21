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

        enum class stream_t
        {
            console,
            file,
            both
        };

        enum class type_t
        {
            info,
            suggestion,
            warning,
            error
        };

        explicit Logger(bool all, bool info, bool suggestions, bool errors, bool warnings);
        explicit Logger();

        void log(const std::string&,
                const Logger::stream_t& = Logger::stream_t::console,
                const Logger::type_t& = Logger::type_t::error) noexcept;

        void init(bool all, bool info, bool suggestions, bool errors, bool warnings);

    private:

        void sendMessage(const std::string& message,
                Logger::stream_t stream,
                std::string& prefix_colored) noexcept;

        void consoleMessage(const std::string&message,
                std::string& prefix,
                std::time_t& time) noexcept;

        void fileMessage(const std::string&,
                std::string& prefix,
                std::time_t& time) noexcept;

        void setPrefix(Logger::type_t type,
                std::string& prefix) noexcept;

        bool log_all;

        bool log_info;
        bool log_suggestions;
        bool log_errors;
        bool log_warnings;
    };
}