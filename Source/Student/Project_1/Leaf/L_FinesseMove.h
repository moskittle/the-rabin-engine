#pragma once
#include "BehaviorNode.h"

class L_FinesseMove : public BaseNode<L_FinesseMove>
{
public:
    L_FinesseMove() = default;
    
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;

    float rotateSpeed = 1.0f;
    float timer = 2.0f;
    float breatheDir = 1.0f;
    Vec3 originalScale = Vec3(3, 3, 3);
};