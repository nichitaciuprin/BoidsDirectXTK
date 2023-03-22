#pragma once
#include "pch.h"
#include "World.h"
#include <Windows.h>
using namespace DirectX::SimpleMath;

World::World()
{
}
void World::Update(float deltaTime)
{
    if(GetKeyState(VK_SHIFT) & 0x8000)
    {
        // https://stackoverflow.com/questions/41600981/how-do-i-check-if-a-key-is-pressed-on-c
        cameraPosition += Vector3::Right*deltaTime;
    }
}