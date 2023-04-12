#pragma once

#include "AABB.h"
#include "Boid.h"

using namespace std;

class BoidWorld
{
public:
    AABB aabb;
    vector<Boid> boids;
    BoidWorld()
    {
        aabb.p0 = Vector3::Backward+Vector3::Left;
        aabb.p1 = Vector3::Forward+Vector3::Right;
        auto size = 50.0f;
        aabb.p0 *= size;
        aabb.p1 *= size;
        aabb.p1 += Vector3::Up*size*2;
        auto subgen = Subgen(0);
        for (int i = 0; i < boidCount; i++)
            boids.push_back(Boid(aabb,subgen));
    }
    void Update(float deltaTime)
    {
        Boid::Update(boids,aabb,deltaTime);
    }
private:
    int boidCount = 400;
};