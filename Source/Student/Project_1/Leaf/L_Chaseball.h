#pragma once
#include "BehaviorNode.h"

class L_ChaseBall : public BaseNode<L_ChaseBall>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt);
    virtual void on_exit() override;

    float defaultSpeed;
    Agent* target;
};