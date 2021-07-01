#pragma once

class NonCopyable
{
protected:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = delete;
    
    NonCopyable& operator=(const NonCopyable&&) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;

    NonCopyable() = default;
};
