#include <pch.h>
#include "L_FootballIdle.h"

L_FootballIdle::L_FootballIdle()
{}

//void L_FootballIdle::on_enter()
//{
//
//    BehaviorNode::on_leaf_enter();
//}

void L_FootballIdle::on_update(float dt)
{
    auto scale3d = agent->get_scaling();
    auto scale = scale3d.x;
    auto breathe = breatheDir * dt;

    agent->set_scaling(scale + breathe);

    if (scale > 3.5)
    {
        breatheDir *= -1;
        agent->set_scaling(3.5);
    }
    else if (scale < 2.5)
    {
        breatheDir *= -1;
        agent->set_scaling(2.5);
    }

    if (timer < 0)
    {
        on_success();
    }

    timer -= dt;

    display_leaf_text();
}

void L_FootballIdle::on_exit()
{
    agent->set_scaling(3);
    timer = 2;
}

void L_FootballIdle::breathe()
{
}
