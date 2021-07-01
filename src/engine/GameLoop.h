#pragma once

#include <cassert>
#include <chrono>
#include <algorithm>
#include <set>
#include <memory>
#include <type_traits>

struct IUpdatable
{
    virtual void Update(float realDeltaTime) = 0;
    virtual ~IUpdatable() = default;
};

struct IFixedUpdatable
{
    virtual void FixedUpdate(float fixedDeltaTime) = 0;
    virtual ~IFixedUpdatable() = default;
};

struct IRenderable
{
    virtual void Render(float interpolation) = 0;
    virtual ~IRenderable() = default;
};

class Time
{
public:
    static size_t GetTicksCount()
    {
        auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    }
};

class GameLoop
{
    template<typename T>
    class GameLoopSet
    {
    public:
        void insert(std::shared_ptr<T> item)
        {
            std::pair <typename std::set<std::shared_ptr<T>>::iterator, bool> pair = m_targets.insert(item);
            assert(pair.second);
        }
        
        bool erase(T* item)
        {
            auto it = m_targets.begin();
            for (; it != m_targets.end();)
            {
                if ((*it).get() == item)
                    it = m_targets.erase(it);
                else
                    ++it;
            }
            return it != m_targets.end();
        }
        
        template<typename F>
        void for_each(F&& f)
        {
            std::for_each(m_targets.begin(), m_targets.end(), f);
        }
        
        ~GameLoopSet()
        {
            assert(m_targets.size() == 0);
        }
        
    private:
        std::set<std::shared_ptr<T>> m_targets;
    };
    
    const int FPS = 30;
    const double MS_PER_UPDATE = 1000.0 / FPS;
    const int SKIP_FRAMES_MAX = 5;
    
public:
    GameLoop()
		: m_lastTime(0.0)
		, m_lag(0.0)
    { }
    
    void Start()
    {
        m_lastTime = Time::GetTicksCount();
        m_lag = 0.0;
    }
    
    void Tick()
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

    template<typename T, typename ... Args, std::enable_if_t<std::is_base_of<IFixedUpdatable, T>::value, bool> = true>
    std::weak_ptr<T> Add(Args&& ... args)
    {
        static_assert(std::is_constructible<T, Args...>::value, "");
        auto item = std::make_shared<T>(std::forward<Args>(args)...);
        m_fixedUpdatables.insert(item);
        return item;
    }

    template<typename T, typename ... Args, std::enable_if_t<std::is_base_of<IUpdatable, T>::value, bool> = true>
    std::weak_ptr<T> Add(Args&& ... args)
    {
        static_assert(std::is_constructible<T, Args...>::value, "");
        auto item = std::make_shared<T>(std::forward<Args>(args)...);
        m_updatables.insert(item);
        return item;
    }

    template<typename T, typename ... Args, std::enable_if_t<std::is_base_of<IRenderable, T>::value, bool> = true>
    std::weak_ptr<T> Add(Args&& ... args)
    {
        static_assert(std::is_constructible<T, Args...>::value, "");
        auto item = std::make_shared<T>(std::forward<Args>(args)...);
        m_renderables.insert(item);
        return item;
    }

    template<typename T, std::enable_if_t<std::is_base_of<IFixedUpdatable, T>::value, bool> = true>
    void Remove(T* item)
    {
        m_fixedUpdatables.erase(item);
    }
    
    template<typename T, std::enable_if_t<std::is_base_of<IFixedUpdatable, T>::value, bool> = true>
    void Remove(std::weak_ptr<T> item)
    {
        auto locked = item.lock();
        assert(locked);
        if (locked)
            m_fixedUpdatables.erase(locked.get());
    }
    
    template<typename T, std::enable_if_t<std::is_base_of<IUpdatable, T>::value, bool> = true>
    void Remove(T* item)
    {
        m_updatables.erase(item);
    }
    
    template<typename T, std::enable_if_t<std::is_base_of<IUpdatable, T>::value, bool> = true>
    void Remove(std::weak_ptr<T> item)
    {
        auto locked = item.lock();
        assert(locked);
        if (locked)
            m_updatables.erase(locked.get());
    }
    
    template<typename T, std::enable_if_t<std::is_base_of<IRenderable, T>::value, bool> = true>
    void Remove(T* item)
    {
        m_renderables.erase(item);
    }
    
    template<typename T, std::enable_if_t<std::is_base_of<IRenderable, T>::value, bool> = true>
    void Remove(std::weak_ptr<T> item)
    {
        auto locked = item.lock();
        assert(locked);
        if (locked)
            m_renderables.erase(locked.get());
    }
    
private:
	double m_lastTime;
	double m_lag;

	GameLoopSet<IUpdatable> m_updatables;
	GameLoopSet<IRenderable> m_renderables;
	GameLoopSet<IFixedUpdatable> m_fixedUpdatables;
};
