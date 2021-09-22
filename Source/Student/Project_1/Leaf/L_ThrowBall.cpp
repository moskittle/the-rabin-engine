#include "pch.h"
#include "L_ThrowBall.h"
#include "../Project1_States.h"

void L_ThrowBall::on_enter()
{
    if (agent->get_ball_state() == BallStates::InAir)
    {
        return;
    }

    agent->set_ball_state(BallStates::InAir);

    printf("throw ball/n");

    on_success();
}

