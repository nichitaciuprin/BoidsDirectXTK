#pragma once

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

struct AABB
{
    Vector3 p0;
    Vector3 p1;
    float MinX() const { return fminf(p0.x,p1.x); }
    float MinY() const { return fminf(p0.y,p1.y); }
    float MinZ() const { return fminf(p0.z,p1.z); }
    float MaxX() const { return fmaxf(p0.x,p1.x); }
    float MaxY() const { return fmaxf(p0.y,p1.y); }
    float MaxZ() const { return fmaxf(p0.z,p1.z); }
    Vector3 Size() const
    {
        Vector3 result = p0-p1;
        result.x = fabs(result.x);
        result.y = fabs(result.y);
        result.z = fabs(result.z);
        return result;
    }
    Vector3 WrapAround(Vector3 point) const
    {
        auto size = Size();
        if      (point.x < MinX()) point.x += size.x;
        else if (point.x > MaxX()) point.x -= size.x;
        if      (point.y < MinY()) point.y += size.y;
        else if (point.y > MaxY()) point.y -= size.y;
        if      (point.z < MinZ()) point.z += size.z;
        else if (point.z > MaxZ()) point.z -= size.z;
        return point;
    }
    Vector3 ShortPathIn(Vector3 point) const
    {
        auto result = Vector3::Zero;
        if      (point.x < MinX()) result.x = MinX() - point.x;
        else if (point.x > MaxX()) result.x = MaxX() - point.x;
        if      (point.y < MinY()) result.y = MinY() - point.y;
        else if (point.y > MaxY()) result.y = MaxY() - point.y;
        if      (point.z < MinZ()) result.z = MinZ() - point.z;
        else if (point.z > MaxZ()) result.z = MaxZ() - point.z;
        return result;
    }
};