#pragma once

#include <memory>
#include "common/NonCopyable.h"

template<typename T>
class Singleton : public NonCopyable
{
public:
    static T& Get();

protected:
    Singleton() = default;
};

template<typename T>
T& Singleton<T>::Get()
{
    static const std::unique_ptr<T> instance{new T};
    return *instance;
}
