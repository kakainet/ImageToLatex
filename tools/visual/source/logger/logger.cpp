#include "logger.hpp"

namespace itl
{
    Logger::Logger(bool all, bool info, bool suggestions, bool errors,
                   bool warnings, bool time)
    {
        Logger::init(all, info, suggestions, errors, warnings, time);
    }

    void Logger::log(const std::string& message, const Logger::stream_t& stream,
                     const Logger::type_t& type) noexcept
    {
        auto& l = Logger::get();
        if(type == type_t::info && !l->log_info) return;
        if(type == type_t::warning && !l->log_warnings) return;
        if(type == type_t::suggestion && !l->log_suggestions) return;
        if(type == type_t::error && !l->log_errors) return;

        std::string prefix_colored;
        l->set_prefix(type, prefix_colored);
        l->send_message(message, stream, prefix_colored);
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
        auto& l = Logger::get();
        l.reset(new Logger());
        auto set_all_to_val = [&l](bool val) {
            l->log_info = val;
            l->log_suggestions = val;
            l->log_errors = val;
            l->log_warnings = val;
        };

        if(all)
        {
            set_all_to_val(true);
            l->log_time = time;
        }
        else
        {
            l->log_all = false;
            l->log_info = info;
            l->log_suggestions = suggestions;
            l->log_errors = errors;
            l->log_warnings = warnings;
            l->log_time = time;
        }
    }

    std::unique_ptr<Logger>& Logger::get() { return Logger::instance; }
}  // namespace itl
