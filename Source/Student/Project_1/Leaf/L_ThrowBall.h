#pragma once
#include "BehaviorNode.h"

class L_ThrowBall : public BaseNode<L_ThrowBall>
{
protected:
    virtual void on_enter() override;

    float defaultSpeed;
};