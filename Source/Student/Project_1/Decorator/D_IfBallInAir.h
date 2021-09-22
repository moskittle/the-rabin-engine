#pragma once
#include "BehaviorNode.h"
#include "../Project1_States.h"

class D_IfBallInAir : public BaseNode<D_IfBallInAir>
{
public:
    D_IfBallInAir();

protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

    Agent* gameAgent;
};