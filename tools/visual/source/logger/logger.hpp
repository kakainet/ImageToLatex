#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "../config/config.hpp"

namespace itl
{
    /**
     * class for logging messages
     */
    class Logger
    {
       public:
        /**
         * enum which describes types of stream
         */
        enum class stream_t
        {
            console, ///< print to stdout
            file, ///< print to file
            both ///< print to both
        };

        /**
         * enum which describes types of stream
         */
        enum class type_t
        {
            info, ///< neutral details of program run
            suggestion, ///< suggestions from authors
            warning, ///< warns if sth isn't safe
            error ///< sth went wrong
        };

        /**
         * @brief explicit ctor
         * @param all - flag if "-la" was passed
         * @param info - flag if "-li" was passed
         * @param suggestions - flag if "-ls" was passed
         * @param errors - flag if "-le" was passed
         * @param warnings - flag if "-lw" was passed
         * @param time - flag if "-lt" was passed
         */
        explicit Logger(bool all, bool info, bool suggestions, bool errors,
                        bool warnings, bool time);

        /**
         * @brief explicit ctor, sets all flags on true
         * without taking into account logging time
         */
        explicit Logger();

        /**
         * @brief prints message
         * @param message - given text to print
         * @param stream - where message should be printed
         * @param type - type of message
         */
        void log(const std::string& message,
                 const Logger::stream_t& stream = Logger::stream_t::console,
                 const Logger::type_t& type = Logger::type_t::error) noexcept;

        /**
         * @brief late initialization of logger
         * @param all - flag if "-la" was passed
         * @param info - flag if "-li" was passed
         * @param suggestions - flag if "-ls" was passed
         * @param errors - flag if "-le" was passed
         * @param warnings - flag if "-lw" was passed
         * @param time - flag if "-lt" was passed
         */
        void init(bool all, bool info, bool suggestions, bool errors,
                  bool warnings, bool time);

       private:

        /**
         * @brief message sender
         * @param message - text to send
         * @param stream - where message should be printed
         * @param prefix_colored - colored prefix of message
         */
        void send_message(const std::string& message, Logger::stream_t stream,
                          std::string& prefix_colored) noexcept;
        /**
         * @brief direct message sender to stdout
         * @param message - text to send
         * @param prefix - prefix of message
         * @param time - time of log
         */
        void console_message(const std::string& message, std::string& prefix,
                             std::time_t& time) noexcept;

        /**
         * @brief
         * @param prefix
         * @param time
         */
        void file_message(const std::string&, std::string& prefix,
                          std::time_t& time) noexcept;

        /**
         * @brief
         * @param type
         * @param prefix
         */
        void set_prefix(Logger::type_t type, std::string& prefix) noexcept;

        bool log_all;
        bool log_info;
        bool log_suggestions;
        bool log_errors;
        bool log_warnings;

        bool log_time;
    };
}  // namespace itl