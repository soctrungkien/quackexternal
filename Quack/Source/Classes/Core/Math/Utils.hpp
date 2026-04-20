#pragma once
#include <cmath>
#include "Vector.hpp"

namespace Math {
    inline float RadToDeg(float rad) {
        return rad * (180.0f / 3.1415926535f);
    }

    inline float DegToRad(float deg) {
        return deg * (3.1415926535f / 180.0f);
    }

    inline Vector3 CalculateAngle(const Vector3 local_pos, const Vector3 target_pos) {
        Vector3 delta = target_pos - local_pos;
        Vector3 angles;

        angles.x = -atan2(delta.y, sqrt(delta.x * delta.x + delta.z * delta.z)) * (180.0f / 3.14159f);
        angles.y = atan2(delta.z, delta.x) * (180.0f / 3.14159f);
        angles.z = 0.0f;
        return angles;
    }

    inline float Distance(const Vector3 a, const Vector3 b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        float dz = a.z - b.z;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    static Vector3 Lerp(const Vector3& a, const Vector3& b, f32 t) {
        t = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;
        return a + (b - a) * t;
    }
}