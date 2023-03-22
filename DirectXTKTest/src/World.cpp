#pragma once
#include "pch.h"
#include "World.h"
using namespace DirectX::SimpleMath;

World::World()
{
}
void World::Update(float deltaTime)
{
    cameraPosition += Vector3::Right*deltaTime;
}