#include "pch.h"
#include "L_QbContain.h"

void L_QbContain::on_enter()
{
    auto currSpeed = agent->get_movement_speed();
    agent->set_movement_speed(currSpeed * 0.5f);

    auto& allAgents = agents->get_all_agents();

    for (auto& a : allAgents)
    {
        if (strcmp(a->get_type(), "Quarterback") == 0)
        {
            target = a;
        }
    }

    on_leaf_enter();
}

void L_QbContain::on_update(float dt)
{

    auto qbPos = target->get_position();
    auto targetPos = agent->get_position();
    targetPos.z  = qbPos.z;

    const auto result = agent->move_toward_point_no_rot(targetPos, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

void L_QbContain::on_exit()
{
    auto currSpeed = agent->get_movement_speed();
    agent->set_movement_speed(currSpeed * 2.0f);
}
