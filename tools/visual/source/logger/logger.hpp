#ifndef ITL_LOGGER_HPP
#define ITL_LOGGER_HPP

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "../config/config.hpp"

class S
{
public:
    static S& getInstance()
    {
        static S    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
private:
    S() {}                    // Constructor? (the {} brackets) are needed here.

    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are unacceptable otherwise you may accidentally get copies of
    // your singleton appearing.
    S(S const&);              // Don't Implement
    void operator=(S const&); // Don't implement

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:

};

namespace itl
{
    /**
     * class for logging messages
     * @warning dispensed with the obligation to logging during initialization itself
     */
    class Logger
    {
       public:
        /**
         * enum which describes types of stream
         */
        enum class stream_t
        {
            console,  ///< print to stdout
            file,     ///< print to file
            both      ///< print to both
        };

        /**
         * enum which describes types of stream
         */
        enum class type_t
        {
            info,        ///< neutral details of program run
            suggestion,  ///< suggestions from authors
            warning,     ///< warns if sth isn't safe
            error        ///< sth went wrong
        };

        static std::unique_ptr<Logger>& get();

        /**
         * @brief deleted ctor
         */
        Logger(Logger const&)     = delete;

        /**
         * @brief deleted assignment operator
         */
        void operator=(S const&)  = delete;

        /**
         * @brief prints message
         * @param message - given text to print
         * @param stream - where message should be printed
         * @param type - type of message
         */
        static void log(const std::string& message,
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
        static void init(bool all, bool info, bool suggestions, bool errors,
                  bool warnings, bool time);

       private:

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
        * @brief default ctor
        */
        Logger() = default;

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
         * @brief direct message sender to file
         * @param message - text to send
         * @param prefix - prefix of message
         * @param time - time of log
         */
        void file_message(const std::string& message, std::string& prefix,
                          std::time_t& time) noexcept;

        /**
         * @brief prepare prefix
         * @param type - type of message
         * @param prefix - string of prefix
         */
        void set_prefix(Logger::type_t type, std::string& prefix) noexcept;

        bool log_all;          ///< flag if ALL messages should be supported
        bool log_info;         ///< flag if [INFO] messages should be supported
        bool log_suggestions;  ///< flag if [SUGGESTION] messages should be
                               ///< supported
        bool log_errors;       ///< flag if [ERROR] messages should be supported
        bool log_warnings;  ///< flag if [WARNING] messages should be supported

        bool log_time;  ///< flag if time should be added to logs

        static inline std::unique_ptr<Logger> instance; ///< singleton instance
    };
}  // namespace itl

#endif