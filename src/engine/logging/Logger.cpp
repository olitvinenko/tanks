#include "Logger.h"

Logger::Logger(std::shared_ptr<spdlog::logger> logger)
 : m_logger(std::move(logger))
{
    assert(m_logger);
}

Logger* Logger::SetLevel(ELevel level)
{
    m_logger->set_level(static_cast<spdlog::level::level_enum>(level));
    return this;
}

Logger* Logger::FlushOn(ELevel level)
{
    m_logger->flush_on(static_cast<spdlog::level::level_enum>(level));
    return this;
}
