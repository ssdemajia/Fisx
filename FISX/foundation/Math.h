#ifndef FISX_MATH_H
#define FISX_MATH_H
#include <math.h>
namespace Fisx
{
    template<typename T>
    class TVector3
    {
    public:
        TVector3() : TVector3(0.0f, 0.0f, 0.0f) {}
        TVector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
        inline TVector3& operator+=(const TVector3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }
        inline TVector3& operator-=(const TVector3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }
        inline TVector3& operator*=(const TVector3& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
        }
        inline TVector3& operator/=(const float& v)
        {
            return *this *= 1.0f / v;
        }
        inline float dot(const TVector3& v) const
        {
            return x * v.x + y * v.y + z * v.z;
        }
        inline float length2() const
        {
            return dot(*this);
        }
        inline float length() const
        {
            return sqrtf(length2());
        }
        inline TVector3& normalize()
        {
            return *this /= length();
        }
        inline TVector3 abs() const
        {
            return TVector3(fabsf(x), fabsf(y), fabsf(z));
        }
        inline T& operator[](int index)
        {
            return (((T*)this)[index]);
        }
        inline const T& operator[](int index) const
        {
            return (((T*)this)[index]);
        }
        T x;
        T y;
        T z;
    };

    template<typename T>
    inline TVector3<T> operator+(const TVector3<T>& v1, const TVector3<T>& v2)
    {
        return TVector3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    template<typename T>
    inline TVector3<T> operator-(const TVector3<T>& v1, const TVector3<T>& v2)
    {
        return TVector3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    template<typename T>
    inline TVector3<T> operator*(const TVector3<T>& v1, const float& v2)
    {
        return TVector3<T>(v1.x * v2, v1.y * v2, v1.z * v2);
    }

    template<typename T>
    inline TVector3<T> operator/(const TVector3<T>& v1, const float& v2)
    {
        return v1 * (1.0 / v2);
    }

    using Vector3 = TVector3<float>;

    class AABB
    {
    public:
        inline Vector3 Center() { return (mMin + mMax) / 2; }
        inline void Expand(const Vector3& e) {
            mMin -= e;
            mMax += e;
        }
        inline bool Contain(const AABB& other)
        {
            return (mMin.x < other.mMin.x) &&
                (mMin.y < other.mMin.y) &&
                (mMin.z < other.mMin.z) &&
                (mMax.x > other.mMax.x) &&
                (mMax.y > other.mMax.y) &&
                (mMax.z > other.mMax.z);
        }
        Vector3 mMin;
        Vector3 mMax;
    };

}
#endif