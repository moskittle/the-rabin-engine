#pragma once
#include "BehaviorNode.h"
#include "../Project1_States.h"

class D_IfMouseClickFar : public BaseNode<D_IfMouseClickFar>
{
public:
    D_IfMouseClickFar();

protected:
    virtual void on_update(float dt) override;

    Agent* gameAgent;
};