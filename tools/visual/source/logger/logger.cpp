#include "logger.hpp"

namespace itl
{
    Logger::Logger(bool all, bool info, bool suggestions, bool errors,
                   bool warnings, bool time)
    {
        this->init(all, info, suggestions, errors, warnings, time);
    }

    Logger::Logger() : Logger(true, true, true, true, true, true) {}

    void Logger::log(const std::string& message, const Logger::stream_t& stream,
                     const Logger::type_t& type) noexcept
    {
        if(type == type_t::info && !this->log_info) return;
        if(type == type_t::warning && !this->log_warnings) return;
        if(type == type_t::suggestion && !this->log_suggestions) return;
        if(type == type_t::error && !this->log_errors) return;

        std::string prefix_colored;
        set_prefix(type, prefix_colored);
        send_message(message, stream, prefix_colored);
    }

    void Logger::send_message(const std::string& message,
                              Logger::stream_t stream,
                              std::string& prefix_colored) noexcept
    {
        std::chrono::time_point<std::chrono::system_clock> date =
            std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(date);

        switch(stream)
        {
            case Logger::stream_t::file:
            {
                file_message(message, prefix_colored, time);
                break;
            }

            case Logger::stream_t::console:
            {
                console_message(message, prefix_colored, time);
                break;
            }

            case Logger::stream_t::both:
            {
                console_message(message, prefix_colored, time);
                file_message(message, prefix_colored, time);
                break;
            }
        }
    }

    void Logger::console_message(const std::string& message,
                                 std::string& prefix,
                                 std::time_t& time) noexcept
    {
        if(log_time)
        {
            std::cout << cst::color::bold_green << std::ctime(&time);
        }

        std::cout << prefix << cst::color::reset << ' ' << message << "\n";
    }

    void Logger::file_message(const std::string& message, std::string& prefix,
                              std::time_t& time) noexcept
    {
        std::ofstream file("data/log/log.txt", std::ios::app);

        file << std::ctime(&time) << prefix << ' ' << message << "\n";
    }

    void Logger::set_prefix(Logger::type_t type, std::string& prefix) noexcept
    {
        std::stringstream prefix_with_color;
        switch(type)
        {
            case Logger::type_t::error:
            {
                prefix_with_color << cst::color::bold_red << "[ERROR]";
                break;
            }

            case Logger::type_t::info:
            {
                prefix_with_color << cst::color::bold_cyan << "[INFO]";
                break;
            }

            case Logger::type_t::suggestion:
            {
                prefix_with_color << cst::color::bold_magenta << "[SUGGESTION]";
                break;
            }

            case Logger::type_t::warning:
            {
                prefix_with_color << cst::color::bold_yellow << "[WARNING]";
                break;
            }
        }

        prefix = prefix_with_color.str();
    }

    void Logger::init(bool all, bool info, bool suggestions, bool errors,
                      bool warnings, bool time)
    {
        auto set_all_to_val = [=](bool val) {
            this->log_info = val;
            this->log_suggestions = val;
            this->log_errors = val;
            this->log_warnings = val;
        };

        if(all)
        {
            set_all_to_val(true);
            this->log_time = time;
        }
        else
        {
            this->log_all = false;
            this->log_info = info;
            this->log_suggestions = suggestions;
            this->log_errors = errors;
            this->log_warnings = warnings;
            this->log_time = time;
        }
    }
}  // namespace itl
