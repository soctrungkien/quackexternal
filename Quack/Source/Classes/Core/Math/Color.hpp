#pragma once
#include <string>
using f32 = float;

struct Color3 final {
    f32 r{ 0.0f };
    f32 g{ 0.0f };
    f32 b{ 0.0f };

    Color3() = default;
    Color3(f32 r, f32 g, f32 b) :
        r(r), g(g), b(b)
    {}
};