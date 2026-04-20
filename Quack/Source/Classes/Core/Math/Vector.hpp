#pragma once
#include <cmath>
#include <cstdint>
#include <algorithm> 

using f32 = float;

struct Vector2 final {
    f32 x{ 0.0f };
    f32 y{ 0.0f };

    Vector2() = default;
    Vector2(f32 x, f32 y) :
        x(x), y(y)
    {}

    f32 Magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 Normalized() const {
        f32 mag = Magnitude();
        if (mag > 0.0f) {
            return *this / mag;
        }
        return Vector2(0, 0);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator/(f32 scalar) const {
        return Vector2(x / scalar, y / scalar);
    }
};

struct Vector3 final {
    f32 x{ 0.0f };
    f32 y{ 0.0f };
    f32 z{ 0.0f };

    Vector3() = default;
    Vector3(f32 x, f32 y, f32 z) :
        x(x), y(y), z(z)
    {}

    Vector3 Normalized() const {
        f32 mag = Magnitude();
        if (mag > 0.0f) {
            return *this / mag;
        }
        return Vector3(0, 0, 0);
    }

    f32 Magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 ToDirection() const {
        f32 pitch = x * (3.14159f / 180.0f);
        f32 yaw = y * (3.14159f / 180.0f);

        return Vector3(
            std::cos(yaw) * std::cos(pitch),
            std::sin(pitch),
            std::sin(yaw) * std::cos(pitch)
        ).Normalized();
    }

    Vector3 ToEulerAngles() const {
        Vector3 normalized = this->Normalized();
        f32 pitch = std::asin(-normalized.y) * (180.0f / 3.14159f);
        f32 yaw = std::atan2(normalized.x, normalized.z) * (180.0f / 3.14159f);

        return Vector3(pitch, yaw, 0.0f);
    }

    f32 Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 Cross(const Vector3 & other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator*(f32 scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator/(f32 scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
};

struct Vector4 final {
    f32 x{ 0.0f };
    f32 y{ 0.0f };
    f32 z{ 0.0f };
    f32 w{ 0.0f };

    Vector4() = default;
    Vector4(f32 x, f32 y, f32 z, f32 w) :
        x(x), y(y), z(z), w(w)
    {}

    Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }
};

struct CFrame {
    Vector3 Right = { 1, 0, 0 };
    Vector3 Up = { 0, 1, 0 };
    Vector3 Back = { 0, 0, 1 };
    Vector3 Position = { 0, 0, 0 };

    CFrame() = default;
    CFrame(Vector3 Right, Vector3 Up, Vector3 Back, Vector3 Position) :
        Right(Right), Up(Up), Back(Back), Position(Position)
    {}
};