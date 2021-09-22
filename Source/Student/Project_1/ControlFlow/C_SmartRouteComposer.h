#pragma once
#include "BehaviorNode.h"

#include <unordered_set>

class C_SmartRouteComposer : public BaseNode<C_SmartRouteComposer>
{
public:
    C_SmartRouteComposer();
protected:
    size_t smartIndex;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;

    std::unordered_set<std::string> exludeRoutes;
private:
    void exlude_routes();
};