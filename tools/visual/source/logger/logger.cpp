#include "logger.hpp"

namespace itl
{
    Logger::Logger(bool all, bool info, bool suggestions, bool errors, bool warnings)
            : log_all(all), log_info(info), log_suggestions(suggestions), log_errors(errors), log_warnings(warnings)
    {
        this->init(all, info, suggestions, errors, warnings);
    }

    Logger::Logger()
        : Logger(true, true, true, true, true)
    {
    }

    void Logger::log(const std::string& message, const Logger::STREAM& stream, const Logger::TYPE& type) noexcept
    {
        if(type == TYPE::INFO && !this->log_info) return;
        if(type == TYPE::WARNING && !this->log_warnings) return;
        if(type == TYPE::SUGGESTION && !this->log_suggestions) return;
        if(type == TYPE::ERROR && !this->log_errors) return;

        std::string prefix_colored;
        setPrefix(type, prefix_colored);
        sendMessage(message, stream, prefix_colored);
    }

    void Logger::sendMessage(const std::string& message, Logger::STREAM stream, std::string &prefix_colored) noexcept
    {
        std::chrono::time_point<std::chrono::system_clock> date = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(date);

        switch (stream)
        {
            case Logger::STREAM::FILE:
            {
                fileMessage(message, prefix_colored, time);
                break;
            }

            case Logger::STREAM::CONSOLE:
            {
                consoleMessage(message, prefix_colored, time);
                break;
            }

            case Logger::STREAM::BOTH:
            {
                consoleMessage(message, prefix_colored, time);
                fileMessage(message, prefix_colored, time);
                break;
            }
        }
    }

    void Logger::consoleMessage(const std::string& message, std::string &prefix, std::time_t& time) noexcept
    {
        std::cout << constants::color::bold_green
                  << std::ctime(&time)
                  << prefix
                  << constants::color::reset
                  << ' '
                  << message
                  << "\n\n";
    }

    void Logger::fileMessage(const std::string& message, std::string &prefix, std::time_t& time) noexcept
    {
        std::ofstream file("data/log/log.txt",std::ios::app);

        file << std::ctime(&time)
             << prefix << ' '
             << message << "\n\n";
    }

    void Logger::setPrefix(Logger::TYPE type, std::string &prefix) noexcept
    {
        std::stringstream prefix_with_color;
        switch (type)
        {
            case Logger::TYPE::ERROR:
            {
                prefix_with_color << constants::color::bold_red
                                  << "[ERROR]";
                break;
            }

            case Logger::TYPE::INFO:
            {
                prefix_with_color << constants::color::bold_cyan
                                  << "[INFO]";
                break;
            }

            case Logger::TYPE::SUGGESTION:
            {
                prefix_with_color << constants::color::bold_magenta
                                  << "[SUGGESTION]";
                break;
            }

            case Logger::TYPE::WARNING:
            {
                prefix_with_color << constants::color::bold_yellow
                                  << "[WARNING]";
                break;
            }
        }

        prefix = prefix_with_color.str();
    }

    void Logger::init(bool all, bool info, bool suggestions, bool errors, bool warnings)
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
        }else
        {
            this->log_all = false;
            this->log_info = info;
            this->log_suggestions = suggestions;
            this->log_errors = errors;
            this->log_warnings = warnings;
        }
    }
}
