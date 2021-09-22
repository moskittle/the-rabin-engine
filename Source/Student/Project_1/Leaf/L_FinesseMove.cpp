#include <pch.h>
#include "L_FinesseMove.h"

void L_FinesseMove::on_enter()
{
    originalScale = agent->get_scaling();
}

void L_FinesseMove::on_update(float dt)
{
    auto currYaw = agent->get_yaw();
    agent->set_yaw(currYaw + (dt * rotateSpeed));

    printf("finesse\n");

    //auto scale3d = agent->get_scaling();
    //auto scale = scale3d.x;
    //auto breathe = breatheDir * dt;

    //agent->set_scaling(scale + breathe);

    //if (scale > 3.5)
    //{
    //    breatheDir *= -1;
    //    agent->set_scaling(3.5);
    //}
    //else if (scale < 2.5)
    //{
    //    breatheDir *= -1;
    //    agent->set_scaling(2.5);
    //}

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
