#pragma once
#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_CheckQbScramble : public BaseNode<L_CheckQbScramble>
{
public:
    L_CheckQbScramble();
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    Vec3 prevQbPosition;
    std::shared_ptr<Agent> target;
};