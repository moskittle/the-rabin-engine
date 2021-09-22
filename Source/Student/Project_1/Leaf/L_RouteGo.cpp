#include <pch.h>
#include "L_RouteGo.h"
#include "Agent/BehaviorAgent.h"

void L_RouteGo::on_enter()
{
    // set animation, speed, etc

    targetPoint = agent->get_position();
    targetPoint += Vec3(0, 0, 10.0f);

    BehaviorNode::on_leaf_enter();
}

void L_RouteGo::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}
