#include "Engine.h"

#include "spdlog/spdlog.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ansicolor_sink.h"

Engine::Engine(std::shared_ptr<IInput> input
               , std::shared_ptr<IClipboard> clipboard
               , std::shared_ptr<IWindow> window)
: m_gameLoop(std::make_unique<GameLoop>())
, m_threadPool(std::make_unique<ThreadPool>())
, m_logManager(std::make_unique<LogManager>())
, m_input(std::move(input))
, m_clipboard(std::move(clipboard))
, m_window(std::move(window))
{
    auto consoleSink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
    consoleSink->set_pattern("%^[%Y-%m-%d %h:%M:%S.%e] %v%$");
    
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/engine.txt", true);
    consoleSink->set_pattern("%^[%Y-%m-%d %h:%M:%S.%e] %v%$");
    
    std::vector<spdlog::sink_ptr> sinks{ consoleSink, file_sink };
    
    m_logManager->MakeLogger("Engine", sinks.begin(), sinks.end())
        ->SetLevel(ELevel::trace)
        ->FlushOn(ELevel::trace);
    
}
