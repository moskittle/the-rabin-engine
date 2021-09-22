#include "pch.h"
#include "L_RushQb.h"

void L_RushQb::on_enter()
{
    auto moveSpeed = agent->get_blackboard().get_value<float>("Move Speed");
    agent->set_movement_speed(defaultSpeed * moveSpeed);

    auto& allAgents = agents->get_all_agents();
    for (auto& a : allAgents)
    {
        if (strcmp(a->get_type(), "Quarterback") == 0)
        {
            target = a;
            break;
        }
    }


    on_leaf_enter();
}

void L_RushQb::on_update(float dt)
{

    const auto result = agent->move_toward_point(target->get_position(), dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

void L_RushQb::on_exit()
{
    //agent->set_movement_speed(defaultSpeed);
}
