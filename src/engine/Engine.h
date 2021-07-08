#pragma once

#include "Singleton.h"

#include "logging/LogManager.h"

#include "GameLoop.h"
#include "threading/ThreadPool.h"

#include <memory>

struct IInput;
struct IClipboard;
struct IWindow;

class Engine : public Singleton<Engine>
{
public:
    Engine(std::shared_ptr<IInput> input, std::shared_ptr<IClipboard> clipboard, std::shared_ptr<IWindow> window);

    std::unique_ptr<GameLoop>&   GetGameLoop()      { return m_gameLoop; }
    std::unique_ptr<ThreadPool>& GetThreadPool()    { return m_threadPool; }
    std::unique_ptr<LogManager>& GetLogManager()    { return m_logManager; }
    
    std::shared_ptr<IInput>     GetInput() const        { return m_input; }
    std::shared_ptr<IClipboard> GetClipboard() const    { return m_clipboard; }
    std::shared_ptr<IWindow>    GetWindow() const       { return m_window; }

private:
    
    std::unique_ptr<GameLoop>   m_gameLoop;
    std::unique_ptr<ThreadPool> m_threadPool;
    std::unique_ptr<LogManager> m_logManager;
    
    std::shared_ptr<IInput>     m_input;
    std::shared_ptr<IClipboard> m_clipboard;
    std::shared_ptr<IWindow>    m_window;
};
