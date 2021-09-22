#include <pch.h>
#include "L_CheckQbScramble.h"
#include "Agent/Agent.h"

L_CheckQbScramble::L_CheckQbScramble()
{
}

void L_CheckQbScramble::on_enter()
{
    // get a list of all current agents
    const auto& allAgents = agents->get_all_agents();

    for (const auto& currAgent : allAgents)
    {
        // make sure it's not our agent
        if (currAgent != agent)
        {
            if (!strcmp(currAgent->get_type(), "Quarterback"))
            {
                target = std::make_shared<Agent>(*currAgent);
                break;
            }
        }
    }

    if (target)
    {
        auto& bb = agent->get_blackboard();
        bb.set_value("Quarterback Agent", target);
        
        if (prevQbPosition == Vec3(0, 0, 0))
        {
            prevQbPosition = target->get_position();
        }

        BehaviorNode::on_leaf_enter();
    }
    else
    {
        on_failure();
    }

}

void L_CheckQbScramble::on_update(float dt)
{
    Vec3 currQbPos = target->get_position();

    if (currQbPos.x != prevQbPosition.x)
    {
        on_success();
    }
    else
    {
        on_failure();
    }

    prevQbPosition = currQbPos;

    display_leaf_text();
}
