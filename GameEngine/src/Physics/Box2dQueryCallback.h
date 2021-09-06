#pragma once
#include <box2d/b2_world_callbacks.h>
#include <vector>
#include <box2d/b2_fixture.h>
#include "PhysicsWorld.h"

class Box2dQueryCallback : public b2QueryCallback
{
public:
    std::vector<b2Body*> FoundBodies;

private:

    virtual bool ReportFixture(b2Fixture* Fixture) override {
        if (Fixture)
        {
            FoundBodies.push_back(Fixture->GetBody());
            return true;
        }
        else
        {
            return false;
        }
    }
};