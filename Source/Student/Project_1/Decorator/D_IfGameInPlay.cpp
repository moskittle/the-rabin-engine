#include "pch.h"

#include "D_IfGameInPlay.h"

D_IfGameInPlay::D_IfGameInPlay()
{
}


void D_IfGameInPlay::on_enter()
{
    const auto& allAgents = agents->get_all_agents();

    for (const auto& a : allAgents)
    {
        if (strcmp(a->get_type(), "Quarterback") == 0)
        {
            gameAgent = a;
        }
    }

    BehaviorNode::on_enter();
}

void D_IfGameInPlay::on_update(float dt)
{
    if (gameAgent->get_game_state() == GameStates::InPlay)
    {
        BehaviorNode* child = children.front();

        child->tick(dt);

        // assume same status as child
        set_status(child->get_status());
        set_result(child->get_result());
    }
    else
    {
        on_failure();
    }

    display_leaf_text();
}
