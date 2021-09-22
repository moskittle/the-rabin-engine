#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_RouteOut : public BaseNode<L_RouteOut>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
};