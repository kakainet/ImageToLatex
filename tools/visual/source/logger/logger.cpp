#include "logger.hpp"

namespace itl
{
    void Logger::log(const std::string& message, const Logger::STREAM& stream, const Logger::TYPE& type) noexcept
    {
        if(type == TYPE::INFO && !this->log_info) return;
        if(type == TYPE::WARNING && !this->log_warnings) return;
        if(type == TYPE::SUGGESTION && !this->log_suggestions) return;
        if(type == TYPE::ERROR && !this->log_errors) return;

        std::string prefix;
        setPrefix(type, prefix);
        sendMessage(message, stream, prefix);
    }

    void Logger::sendMessage(const std::string& message, Logger::STREAM stream, std::string &prefix) noexcept
    {
        std::chrono::time_point<std::chrono::system_clock> date = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(date);

        switch (stream)
        {
            case Logger::STREAM::FILE:
            {
                fileMessage(message, prefix, time);
                break;
            }

            case Logger::STREAM::CONSOLE:
            {
                consoleMessage(message, prefix, time);
                break;
            }

            case Logger::STREAM::BOTH:
            {
                consoleMessage(message, prefix, time);
                fileMessage(message, prefix, time);
                break;
            }
        }
    }

    void Logger::consoleMessage(const std::string& message, std::string &prefix, std::time_t& time) noexcept
    {
        std::cout << std::ctime(&time);
        std::cout << prefix << ' ';
        std::cout << message << "\n\n";
    }

    void Logger::fileMessage(const std::string& message, std::string &prefix, std::time_t& time) noexcept
    {
        std::ofstream file("data/log/log.txt",std::ios::app);

        file << std::ctime(&time);
        file << prefix << ' ';
        file << message << "\n\n";
    }

    void Logger::setPrefix(Logger::TYPE type, std::string &prefix) noexcept
    {
        switch (type)
        {

            case Logger::TYPE::ERROR:
            {
                prefix = "[ERROR]";
                break;
            }


            case Logger::TYPE::INFO:
            {
                prefix = "[INFO]";
                break;
            }

            case Logger::TYPE::SUGGESTION:
            {
                prefix = "[SUGGESTION]";
                break;
            }

            case Logger::TYPE::WARNING:
            {
                prefix = "[WARNING]";
                break;
            }
        }
    }

    Logger::Logger(bool all, bool info, bool suggestions, bool errors, bool warnings)
        : log_all(all), log_info(info), log_suggestions(suggestions), log_errors(errors), log_warnings(warnings)
    {
        auto set_all_to_val = [=](bool val)
        {
            this->log_info = val;
            this->log_suggestions = val;
            this->log_errors = val;
            this->log_warnings = val;
        };

        if(all && (!info || !suggestions || !errors || !warnings))
        {

            set_all_to_val(true);
            this->log(constants::flags::contradiction_flags ,Logger::STREAM::BOTH, Logger::TYPE::WARNING);
        }

        if(all)
        {
            set_all_to_val(true);
        }
    }
}
