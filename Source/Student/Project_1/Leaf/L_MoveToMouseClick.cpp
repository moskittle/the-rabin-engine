#include <pch.h>
#include "L_MoveToMouseClick.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToMouseClick::on_enter()
{
    // set animation, speed, etc

    // grab the target position from the blackboard
    const auto &bb = agent->get_blackboard();
    targetPoint = bb.get_value<Vec3>("Click Position");

    //agent->look_at_point(targetPoint);
    set_game_state_in_game();

	BehaviorNode::on_leaf_enter();

}

void L_MoveToMouseClick::on_update(float dt)
{
    Vec3 targetPos = targetPoint;
    targetPos.y = strcmp("Johnny Football", agent->get_type()) == 0 ? targetPoint.y : targetPoint.y + 5;

    const auto result = agent->move_toward_point(targetPos, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

void L_MoveToMouseClick::on_exit()
{
    auto allAgent = agents->get_all_agents();

    if (strcmp("Johnny Football", agent->get_type()) == 0)
    {
        const auto& allAgents = agents->get_all_agents();

        agent->set_game_state(GameStates::PostPlay);
    }

    if (strcmp("Receiver", agent->get_type()) == 0)
    {
        const auto& allAgents = agents->get_all_agents();

        agent->set_ball_state(BallStates::InWRsHand);

        for (auto& a : allAgents)
        {
            a->set_movement_speed(0);
        }
    }
}


void L_MoveToMouseClick::set_game_state_in_game()
{
    if (strcmp("Quarterback", agent->get_type()) == 0)
    {
        const auto& allAgents = agents->get_all_agents();

        agent->set_game_state(GameStates::InPlay);
    }

}