#pragma once
#include "pch.h"
using namespace DirectX::SimpleMath;

class World
{
public:
    World();
    void Update(float deltaTime);
private:
    Vector3 cameraPosition;
};
