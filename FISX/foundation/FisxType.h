#ifndef _FISX_TYPE_H
#define _FISX_TYPE_H
namespace Fisx
{
    template<typename T>
    class Vector3
    {
    public:
        Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
        T x;
        T y;
        T z;
    };
}
#endif