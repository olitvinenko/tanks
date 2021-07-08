#include "LogManager.h"

LogManager::~LogManager()
{
    spdlog::shutdown();
}

LogManager::logger_ptr LogManager::GetLogger(const std::string& name)
{
    auto logger = spdlog::get(name);
    if (!logger)
        return nullptr;
    
    return std::shared_ptr<Logger>(new Logger(logger));
}
