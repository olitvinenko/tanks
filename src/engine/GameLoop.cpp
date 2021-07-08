#include "GameLoop.h"

size_t Time::GetTicksCount()
{
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}


GameLoop::GameLoop()
    : m_lastTime(0.0)
    , m_lag(0.0)
{ }
    
GameLoop::~GameLoop()
{
    m_fixedUpdatables.clear();
    m_updatables.clear();
    m_renderables.clear();
}

void GameLoop::Start()
{
    m_lastTime = Time::GetTicksCount();
    m_lag = 0.0;
}

void GameLoop::Tick()
{
    double current = Time::GetTicksCount();
    double elapsed = current - m_lastTime;
    
    m_lastTime = current;
    m_lag += elapsed;
    
    float realElapsedSec = elapsed / 1000.0;
    m_updatables.for_each([realElapsedSec](std::shared_ptr<IUpdatable> f) { f->Update(realElapsedSec); });
    
    int loops = 0;
    while (m_lag >= MS_PER_UPDATE && loops < SKIP_FRAMES_MAX)
    {
        float fixedDeltaTime = MS_PER_UPDATE / 1000.0;
        m_fixedUpdatables.for_each([fixedDeltaTime](std::shared_ptr<IFixedUpdatable> f) { f->FixedUpdate(fixedDeltaTime); });
        
        m_lag -= MS_PER_UPDATE;
        
        //std::cout << elapsed << " " << fixedDeltaTime << "  " << m_lag << "  " << loops << std::endl;
        loops++;
    }
    
    float interpolation = m_lag / MS_PER_UPDATE;
    
    m_renderables.for_each([interpolation](std::shared_ptr<IRenderable> f) { f->Render(interpolation); });
}
