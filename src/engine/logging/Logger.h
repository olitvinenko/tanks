#pragma once

#include <memory>
#include <string>

#include "spdlog/spdlog.h"

enum ELevel
{
    trace = spdlog::level::level_enum::trace,
    debug,
    info,
    warn,
    err,
    critical,
    off,
    n_levels
};

class Logger
{
    friend class LogManager;
private:
    explicit Logger(std::shared_ptr<spdlog::logger> logger);
    
public:
    
    void SetLevel(ELevel level);
    void FlushOn(ELevel level);
    
    ELevel GetLevel() const { return static_cast<ELevel>(m_logger->level()); }
    
    template<typename ... Args>
    void Trace(const std::string& format, Args&& ... args)
    {
        m_logger->trace(format, std::forward<Args>(args)...);
    }
    
    template<typename ... Args>
    void Debug(const std::string& format, Args&& ... args)
    {
        m_logger->debug(format, std::forward<Args>(args)...);
    }
    
    template<typename ... Args>
    void Info(const std::string& format, Args&& ... args)
    {
        m_logger->info(format, std::forward<Args>(args)...);
    }
    
    template<typename ... Args>
    void Warn(const std::string& format, Args&& ... args)
    {
        m_logger->warn(format, std::forward<Args>(args)...);
    }
    
    template<typename ... Args>
    void Error(const std::string& format, Args&& ... args)
    {
        m_logger->error(format, std::forward<Args>(args)...);
    }
    
    template<typename ... Args>
    void Fatal(const std::string& format, Args&& ... args)
    {
        m_logger->critical(format, std::forward<Args>(args)...);
    }
    
private:
    std::shared_ptr<spdlog::logger> m_logger;
};
