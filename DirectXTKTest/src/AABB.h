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
};