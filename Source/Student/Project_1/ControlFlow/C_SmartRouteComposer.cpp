#include <pch.h>
#include "C_SmartRouteComposer.h"

C_SmartRouteComposer::C_SmartRouteComposer()
{
}

void C_SmartRouteComposer::on_enter()
{
    exlude_routes();

    smartIndex = RNG::range(static_cast<size_t>(0), children.size() - 1);

    // regenerate if route is not smart
    while (exludeRoutes.count(children[smartIndex]->get_name()))
    {
        smartIndex = RNG::range(static_cast<size_t>(0), children.size() - 1);
    }

    BehaviorNode::on_enter();
}

void C_SmartRouteComposer::on_update(float dt)
{
    // if any child succeeds, the node succeeds
    // if all children fail, the node fails

    // regenerate if route is not smart
    while (exludeRoutes.count(children[smartIndex]->get_name()))
    {
        smartIndex = RNG::range(static_cast<size_t>(0), children.size() - 1);
    }

    BehaviorNode* currentNode = children[smartIndex];

    currentNode->tick(dt);

    if (currentNode->succeeded() == true)
    {
        on_success();
    }
}

void C_SmartRouteComposer::on_exit()
{
    exludeRoutes.clear();
}

void C_SmartRouteComposer::exlude_routes()
{
    auto currPos = agent->get_position();

    if (currPos.z > 80.0f || currPos.z < 20.0f)
    {
        exludeRoutes.insert("L_RouteOut");
    }

    if (currPos.x > 80.0f)
    {
        exludeRoutes.insert("L_RouteGo");
    }

    if (currPos.z < 60.0f && currPos.z > 40.0f)
    {
        exludeRoutes.insert("L_RouteIn");
    }
}