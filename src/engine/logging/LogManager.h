#pragma once

#include <memory>
#include <string>

#include "Logger.h"

#include "spdlog/spdlog.h"

class LogManager
{
    using logger_ptr = std::shared_ptr<Logger>;

public:
    ~LogManager();
    
    template<typename It>
    logger_ptr MakeLogger(const std::string& name, It begin, It end)
    {
        auto logger = std::make_shared<spdlog::logger>(name, begin, end);
        spdlog::register_logger(logger);
        return std::shared_ptr<Logger>(new Logger(logger));
    }

    logger_ptr GetLogger(const std::string& name);

    template<typename ... Args>
    void Trace(const std::string& name, const std::string& format, Args&& ... args)
    {
        logger_ptr logger = GetLogger(name);
        if (!logger)
            return;

        logger->Trace(format, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    void Debug(const std::string& name, const std::string& format, Args&& ... args)
    {
        logger_ptr logger = GetLogger(name);
        if (!logger)
            return;

        logger->Debug(format, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    void Info(const std::string& name, const std::string& format, Args&& ... args)
    {
        logger_ptr logger = GetLogger(name);
        if (!logger)
            return;

        logger->Info(format, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    void Warn(const std::string& name, const std::string& format, Args&& ... args)
    {
        logger_ptr logger = GetLogger(name);
        if (!logger)
            return;

        logger->Warn(format, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    void Error(const std::string& name, const std::string& format, Args&& ... args)
    {
        logger_ptr logger = GetLogger(name);
        if (!logger)
            return;

        logger->Error(format, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    void Fatal(const std::string& name, const std::string& format, Args&& ... args)
    {
        logger_ptr logger = GetLogger(name);
        if (!logger)
            return;

        logger->Fatal(format, std::forward<Args>(args)...);
    }
};
