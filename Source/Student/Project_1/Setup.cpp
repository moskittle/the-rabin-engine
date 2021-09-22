#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"
#include <memory>

void ProjectOne::setup()
{
    // Create your inital agents
    //auto exampleAgent = agents->create_behavior_agent("ExampleAgent", BehaviorTreeTypes::Example);

    Vec3 blueTeamColor = Vec3(0.1f, 0.1f, 0.8f);
    Vec3 redTeamColor = Vec3(0.8f, 0.1f, 0.1f);

    auto game = agents->create_behavior_agent("Game Day", BehaviorTreeTypes::GameState);
    game->set_scaling(0.01);
    game->set_color(Vec3(0, 0, 0));

    auto ball = agents->create_behavior_agent("Johnny Football", BehaviorTreeTypes::BallState);
    ball->set_position(Vec3(35.0f, 5.0f, 50.0f));
    ball->set_pitch(0.8);
    ball->set_scaling(Vec3(3, 1, 3));
    ball->set_color(Vec3(0, 1, 0));
    auto& ball_bb = ball->get_blackboard();
    ball_bb.set_value("Move Speed", 30.0f);

    auto leftOT = agents->create_behavior_agent("Offensive Tackle", BehaviorTreeTypes::OLineman);
    leftOT->set_name("OT");
    leftOT->set_position(Vec3(35.0f, 0.0f, 50.0f));
    leftOT->set_color(blueTeamColor);
    leftOT->set_yaw(-1.8);
    
    auto qb = agents->create_behavior_agent("Quarterback", BehaviorTreeTypes::Quarterback);
    qb->set_name("QB");
    qb->set_position(Vec3(20.0f, 0.0f, 50.0f));
    qb->set_color(blueTeamColor);
    qb->set_yaw(-1.8);

    auto mlb = agents->create_behavior_agent("Middle Linebacker", BehaviorTreeTypes::Linebacker);
    mlb->set_name("MLB");
    mlb->set_position(Vec3(50.0f, 0.0f, 50.0f));
    mlb->set_color(redTeamColor);
    mlb->set_yaw(-1.8);
    auto& mlb_bb = mlb->get_blackboard();
    mlb_bb.set_value("Move Speed", 5.0f);

    auto wr = agents->create_behavior_agent("Receiver", BehaviorTreeTypes::Receiver);
    wr->set_name("WR");
    wr->set_color(blueTeamColor);
    wr->set_position(Vec3(35.0f, 0.0f, 75.0f));
    wr->set_yaw(-1.8);


    // you can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code that yourself
    // that's the realm of project 2 though
    terrain->goto_map(1);

    // you can also enable the pathing layer and set grid square colors as you see fit
    // works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    // camera position can be modified from this default as well
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees
}

