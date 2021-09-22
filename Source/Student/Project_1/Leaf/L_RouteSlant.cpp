#include <pch.h>
#include "L_RouteSlant.h"
#include "Agent/BehaviorAgent.h"

void L_RouteSlant::on_enter()
{
    // set animation, speed, etc

    targetPoint = agent->get_position();
    auto dir = targetPoint.z > 50.0f ? -1.0f : 1.0f;

    targetPoint += Vec3(10.0f, 0, 10.0f * dir);

    BehaviorNode::on_leaf_enter();
}

void L_RouteSlant::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}
