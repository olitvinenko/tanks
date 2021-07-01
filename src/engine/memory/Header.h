#pragma once

#include <stddef.h>

using finalize_func = void (*)(void*);

struct Header
{
    finalize_func func;
    size_t count;
};

template<typename T>
Header& ExtractFrom(T* p)
{
    return *(reinterpret_cast<Header*>(p) - 1);
}
