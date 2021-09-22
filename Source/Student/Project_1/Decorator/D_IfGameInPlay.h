#pragma once
#include "BehaviorNode.h"
#include "../Project1_States.h"

class D_IfGameInPlay : public BaseNode<D_IfGameInPlay>
{
public:
    D_IfGameInPlay();

protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

    Agent* gameAgent;
};