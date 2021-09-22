#pragma once
#include "BehaviorNode.h"

class L_FootballIdle : public BaseNode<L_FootballIdle>
{
public:
    L_FootballIdle();

protected:
    //virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;
private:
    void breathe();

    float breatheDir = 1;
    float timer = 2;
};
