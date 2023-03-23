#pragma once
#include "pch.h"
#include "World.h"
#include <Windows.h>
using namespace DirectX::SimpleMath;
using namespace std;

Vector2 m_direction;

World::World()
{
}
void World::Update(float deltaTime)
{
    // SHORT state = GetKeyState(VK_SHIFT);
    // SHORT state2 = GetKeyState(VK_SHIFT) & 0x8000;
    // SHORT test = 0x0001;
    // auto isHightOrderBit = (GetKeyState(VK_SHIFT) >> 15);
    // if (GetKeyState(VK_SHIFT) & 0x8000)
    // if(isHightOrderBit)
    // {
    //     // https://stackoverflow.com/questions/41600981/how-do-i-check-if-a-key-is-pressed-on-c
    //     cameraPosition += Vector3::Right*deltaTime;
    // }
    cameraPosition += Vector3(m_direction.x,m_direction.y,0) * deltaTime;
}
void World::MoveCamera(Vector2 direction)
{
    //m_direction = Vector2(1, 1);
    m_direction = direction;
    //cameraPosition += direction*deltaTime;
}