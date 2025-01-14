#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_MoveToMouseClick : public BaseNode<L_MoveToMouseClick>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;

    void set_game_state_in_game();

private:
    Vec3 targetPoint;
};