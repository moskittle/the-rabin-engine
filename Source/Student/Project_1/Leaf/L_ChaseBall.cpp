#include "pch.h"
#include "L_ChaseBall.h"

void L_ChaseBall::on_enter()
{
    defaultSpeed = agent->get_movement_speed();
    agent->set_movement_speed(defaultSpeed * 1.2f);

    auto& allAgents = agents->get_all_agents();
    for(auto& a : allAgents)
    {
        if (strcmp(a->get_type(), "Johnny Football") == 0)
        {
            target = a;
            break;
        }
    }

    on_leaf_enter();
}

void L_ChaseBall::on_update(float dt)
{
    const auto result = agent->move_toward_point(target->get_position(), dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

void L_ChaseBall::on_exit()
{
    agent->set_movement_speed(defaultSpeed);
}
