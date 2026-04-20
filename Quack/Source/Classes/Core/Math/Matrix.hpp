#pragma once
#include <cstdint>
#include <cstring>

using f32 = float;

struct Matrix3 final {
    f32 data[9]{};

    Matrix3() = default;
    Matrix3(const f32 input[9]) {
        std::memcpy(data, input, sizeof(data));
    }
};

struct Matrix4 final {
    f32 data[16]{};

    Matrix4() = default;
    Matrix4(const f32 input[16]) {
        std::memcpy(data, input, sizeof(data));
    }
};