#include "pch.h"

#include "D_IfGetBlocked.h"

D_IfGetBlocked::D_IfGetBlocked()
{
}


void D_IfGetBlocked::on_enter()
{
    const auto& allAgents = agents->get_all_agents();

    for (const auto& a : allAgents)
    {
        if (strcmp(a->get_type(), "Offensive Tackle") == 0)
        {
            olines.push_back(a);
        }
    }

    closestOline = nullptr;

    BehaviorNode::on_enter();
}

void D_IfGetBlocked::on_update(float dt)
{

    float minDist = std::numeric_limits<float>().max();
    for (auto oline : olines)
    {
        float dist = Vec3::Distance(oline->get_position(), agent->get_position());

        if (dist < minDist)
        {
            minDist = dist;
            closestOline = oline;
        }
    }

    if (closestOline != nullptr && minDist < 3.0f)
    {
        BehaviorNode* child = children.front();

        child->tick(dt);

        on_success();
    }
    //else
    //{
    //    on_failure();
    //}

    display_leaf_text();
}
