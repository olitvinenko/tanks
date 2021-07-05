#pragma once

#include "common/Types.h"

struct Color
{
    union {
        uint8 rgba[4];
        uint32 color;
        struct {
            uint8 r;
            uint8 g;
            uint8 b;
            uint8 a;
        };
    };

    Color() = default;
    Color(uint32 c) : color(c) {}
    Color(uint8 r_, uint8 g_, uint8 b_, uint8 a_)
        : r(r_)
        , g(g_)
        , b(b_)
        , a(a_)
    {}
};
