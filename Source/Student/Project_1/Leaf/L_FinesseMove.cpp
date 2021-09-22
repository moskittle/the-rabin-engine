#include <pch.h>
#include "L_FinesseMove.h"

void L_FinesseMove::on_enter()
{
    originalScale = agent->get_scaling();

    on_leaf_enter();
}

void L_FinesseMove::on_update(float dt)
{
    //auto currYaw = agent->get_yaw();
    //agent->set_yaw(currYaw + (dt * rotateSpeed));

    auto currYaw = agent->get_roll();
    agent->set_roll(currYaw + (dt * rotateSpeed));

    if (timer < 0)
    {
        on_success();
    }

    timer -= dt;
}

void L_FinesseMove::on_exit()
{
    agent->set_scaling(originalScale);
}
