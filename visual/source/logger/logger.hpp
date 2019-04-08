#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>

namespace itl
{
    /**
    * @brief Tool for sending messages
    */
    class Logger
    {
        /* ===Objects=== */
    public:
        /**
        * @brief type of streaming - sending messages
        */
        enum class STREAM
        {
            CONSOLE,
            FILE,
            BOTH
        };

        /**
        * @brief type of information
        */
        enum class TYPE
        {
            INFO,
            SUGGESTION ,
            WARNING,
            ERROR
        };
        /* ===Methods=== */
    public:
        /**
        * @brief Method for sending messages
        * @param message - contents
        * @param stream - type of stream
        * @param type - kind of message
        * @see STREAM
        * @see TYPE
        */
        static void Log(const std::string&,
                const Logger::STREAM& = Logger::STREAM::CONSOLE,
                const Logger::TYPE& = Logger::TYPE::ERROR) noexcept;
    private:

        /**
        * @brief sends message
        * @param message - contents
        * @param stream - type of stream
        * @param prefix of message
        * @see STREAM
        * @see TYPE
        */
        static void sendMessage(const std::string& message,
                Logger::STREAM stream,
                std::string& prefix) noexcept;

        /**
        * @brief sends message to console
        * @param message - contents
        * @param prefix of message
        * @param time of message
        */
        static void consoleMessage(const std::string&message,
                std::string& prefix,
                std::time_t& time) noexcept;

        /**
        * @brief sends message to file
        * @param message - contents
        * @param prefix of message
        * @param time of message
        */
        static void fileMessage(const std::string&,
                std::string& prefix,
                std::time_t& time) noexcept;

        /**
        * @brief sends message to console
        * @param prefix of message
        * @param type of message
        * @see TYPE
        */
        static void setPrefix(Logger::TYPE type,
                std::string& prefix) noexcept;
    };
}