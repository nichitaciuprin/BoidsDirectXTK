#pragma once

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace Vector3Ext
{
    static Vector3 ClampLength(Vector3 vector, float min, float max)
    {
        auto length = vector.Length();
        if (length > max) return vector*(max/length);
        if (length < min) return vector*(min/length);
        return vector;
    }
    static Vector3 MoveTowards(Vector3 fromVector, Vector3 toVector, float delta)
    {
        if (fromVector == toVector) return fromVector;
        auto diff = toVector-fromVector;
        auto dist = diff.Length();
        if (dist <= delta) return toVector;
        diff.Normalize();
        auto moveVec = diff*delta;
        return fromVector+moveVec;
    }
    static Vector3 PositionUpdateAdvanced(Vector3 position, Vector3 oldVelocity, Vector3 newVelocity, float deltaTime)
    {
        return position + (oldVelocity+newVelocity)/2 * deltaTime;
    }
}