#include "pch.h"
#include "D_IfMouseClickFar.h"

D_IfMouseClickFar::D_IfMouseClickFar()
{
}

void D_IfMouseClickFar::on_update(float dt)
{
    auto& bb = agent->get_blackboard();
    auto clickPos = bb.get_value<Vec3>("Click Position");

    if (clickPos.x > 50)
    {
        BehaviorNode* child = children.front();

        child->tick(dt);

        on_success();
    }
    else
    {
        on_failure();
    }

    display_leaf_text();
}
