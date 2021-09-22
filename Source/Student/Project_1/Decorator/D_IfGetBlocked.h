#pragma once
#include "BehaviorNode.h"
#include "../Project1_States.h"
#include <vector>

class D_IfGetBlocked : public BaseNode<D_IfGetBlocked>
{
public:
    D_IfGetBlocked();

protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

    std::vector<Agent*> olines;
    Agent* closestOline;
};