#pragma once

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

struct AABB
{
    Vector3 p0;
    Vector3 p1;
    float MinX() { return fminf(p0.x,p1.x); }
    float MinY() { return fminf(p0.y,p1.y); }
    float MinZ() { return fminf(p0.z,p1.z); }
    float MaxX() { return fmaxf(p0.x,p1.x); }
    float MaxY() { return fmaxf(p0.y,p1.y); }
    float MaxZ() { return fmaxf(p0.z,p1.z); }
    Vector3 Size()
    {
        Vector3 result = p0-p1;
        result.x = fabs(result.x);
        result.y = fabs(result.y);
        result.z = fabs(result.z);
        return result;
    }
};