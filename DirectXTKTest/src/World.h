#pragma once
#include "pch.h"
using namespace DirectX::SimpleMath;

class World
{
public:
    Vector3 cameraPosition = Vector3::Backward*5;

    World();
    void Update(float deltaTime);
    void MoveCamera(Vector2 direction);
private:
};
