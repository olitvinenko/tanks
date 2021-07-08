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

struct Time
{
    static size_t GetTicksCount();
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
        
        void clear()
        {
            m_targets.clear();
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
    GameLoop();
    
    ~GameLoop();
    
    void Start();
    
    void Tick();

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
