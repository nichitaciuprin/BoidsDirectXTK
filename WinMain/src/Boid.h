#pragma once

#include "DirectXTKWrapper.h"

class Boid
{
public:
    Vector3 pos;
    Vector3 vel;
    Boid(const AABB& aabb)
    {
        auto randPointInsideAABB = Vector3
        (
            XTKW::RandomRange(aabb.MinX(),aabb.MaxX()),
            XTKW::RandomRange(aabb.MinY(),aabb.MaxY()),
            XTKW::RandomRange(aabb.MinZ(),aabb.MaxZ())
        );
        pos = randPointInsideAABB;

        auto xRand = XTKW::RandomFractionSigned();
        auto yRand = XTKW::RandomFractionSigned();
        auto zRand = XTKW::RandomFractionSigned();
        auto randDirection = Vector3(xRand,yRand,zRand);
        randDirection.Normalize();
        auto randSpeed = XTKW::RandomRange(minSpeed,maxSpeed);
        vel = randDirection * randSpeed;
    }
    static void Update(vector<Boid>& boids, const AABB& aabb, float deltaTime)
    {
        auto length = boids.size();

        for (int i = 0;i < length; i++)
        {
            Boid& boid = boids[i];
            boid.vec_1 = Vector3::Zero;
            boid.vec_2 = Vector3::Zero;
            boid.vec_3 = Vector3::Zero;
            boid.count_1 = 0;
            boid.count_2 = 0;
        }

        // ALL UNIQUE PAIRS
        for (int i = 0;   i < length; i++)
        for (int j = i+1; j < length; j++)
        {
            CalculatePair(boids[i],boids[j]);
        }

        for (int i = 0; i < length; i++)
        {
            Boid& boid = boids[i];
            auto targetVelocity = boid.TargetVelocity(aabb);
            auto newVelocity = Vector3Ext::MoveTowards(boid.vel,targetVelocity,acc*deltaTime);
            boid.pos = Vector3Ext::PositionUpdateAdvanced(boid.pos,boid.vel,boid.vel,deltaTime);
            boid.vel = newVelocity;
        }
    }
private:
    static const float minSpeed;
    static const float maxSpeed;
    static const float range_1;
    static const float range_2;
    static const float range_3;
    static const float rangeSquared_1;
    static const float rangeSquared_2;
    static const float rangeSquared_3;
    static const float power1;
    static const float power2;
    static const float power3;
    static const float acc;
    Vector3 vec_1;
    Vector3 vec_2;
    Vector3 vec_3;
    int count_1;
    int count_2;
    static void CalculatePair(Boid& boid1, Boid& boid2)
    {
        auto diff = boid1.pos-boid2.pos;
        auto distSquared = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
        auto dist = sqrtf(distSquared);

        // COHESION
        if (distSquared >= rangeSquared_1) return;

        boid1.vec_1 = boid1.vec_1+boid2.pos; boid1.count_1++;
        boid2.vec_1 = boid2.vec_1+boid1.pos; boid2.count_1++;

        // ALIGHMENT
        if (distSquared >= rangeSquared_2) return;

        boid1.vec_2 = boid1.vec_2+boid2.vel; boid1.count_2++;
        boid2.vec_2 = boid2.vec_2+boid1.vel; boid2.count_2++;

        // SEPARATION
        if (distSquared >= rangeSquared_3) return;

        auto normFraction = 1.0f/dist;
        auto normDiff = Vector3(diff.x*normFraction,diff.y*normFraction,diff.z*normFraction);
        auto dist2 = range_3 - dist;
        normDiff = normDiff*dist2;

        boid1.vec_3 = boid1.vec_3+normDiff;
        boid2.vec_3 = boid2.vec_3-normDiff;
    }
    Vector3 TargetVelocity(const AABB& aabb) const
    {
        auto l_vec_1 = vec_1;
        auto l_vec_2 = vec_2;
        auto l_vec_3 = vec_3;

        if (count_1 != 0)
        {
            l_vec_1 /= (float)count_1;
            l_vec_1 -= pos;
        }
        if (count_2 != 0)
        {
            l_vec_2 /= (float)count_2;
            l_vec_2 -= vel;
        }

        l_vec_1 *= power1;
        l_vec_2 *= power2;
        l_vec_3 *= power3;

        auto result = vel + l_vec_1 + l_vec_2 + l_vec_3;

        result += aabb.ShortPathIn(pos);

        result = Vector3Ext::ClampLength(result,minSpeed,maxSpeed);
        return result;
    }
};
const float Boid::minSpeed = 9;
const float Boid::maxSpeed = 15;
const float Boid::range_1 = 5;
const float Boid::range_2 = 5;
const float Boid::range_3 = 2;
const float Boid::rangeSquared_1 = range_1*range_1;
const float Boid::rangeSquared_2 = range_2*range_2;
const float Boid::rangeSquared_3 = range_3*range_3;
const float Boid::power1 = 0.01;
const float Boid::power2 = 0.01;
const float Boid::power3 = 0.04;
const float Boid::acc = 10;