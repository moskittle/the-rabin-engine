#pragma once
#include "BehaviorNode.h"

class L_QbContain : public BaseNode<L_QbContain>
{
protected:

    virtual void on_enter();
    virtual void on_update(float dt);
    virtual void on_exit();
private:
    float minTime = 1.0f;
    Agent* target;
};