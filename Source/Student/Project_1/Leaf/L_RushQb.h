#pragma once
#include "BehaviorNode.h"

class L_RushQb : public BaseNode<L_RushQb>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt);
    virtual void on_exit() override;

    float defaultSpeed = 1.0f;
    Agent* target;
};