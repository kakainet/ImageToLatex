#include "logger.hpp"

namespace itl
{
    void Logger::log(const std::string& message, const Logger::STREAM& stream, const Logger::TYPE& type) noexcept
    {
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
}
