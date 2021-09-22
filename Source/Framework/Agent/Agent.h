/******************************************************************************/
/*!
\file		Agent.h
\project	CS380/CS580 AI Framework
\author		Dustin Holmes
\summary	Base actor declarations

Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../Misc/NiceTypes.h"
#include <string>
#include "../Student/Project_1/Project1_States.h"

// forward declarations
class AgentOrganizer;

class Agent
{
    friend class AgentOrganizer;
public:
    Agent(const char *type, size_t id);

#pragma region Getters
    const Vec3 &get_position() const;
    const Vec3 &get_scaling() const;

    Vec3 get_forward_vector() const;
    Vec3 get_right_vector() const;
    Vec3 get_up_vector() const;

    // in radians
    float get_pitch() const;
    float get_yaw() const;
    float get_roll() const;

    const Vec3 &get_color() const;
    
    const Mat4 &get_local_to_world();

    const char *get_type() const;
    const size_t &get_id() const;
    const std::string& get_name() const;
    const GameStates& get_game_state() const;
    const BallStates& get_ball_state() const;

    float get_movement_speed() const;
#pragma endregion

#pragma region Setters
    void set_position(const Vec3 &pos);
    void set_scaling(const Vec3 &scale);
    void set_scaling(float scalar);

    void set_pitch(float angleRadians);
    void set_yaw(float angleRadians);
    void set_roll(float angleRadians);
    void set_pitch_directly(float angleRadians);
    void set_yaw_directly(float angleRadians);
    void set_roll_directly(float angleRadians);

    void set_color(const Vec3 &newColor);
    void set_name(std::string _name);
    void set_game_state(GameStates state);
    void set_ball_state(BallStates state);
    
    void set_movement_speed(float speed);
#pragma endregion

    virtual void update(float dt);

private:
    Vec3 position;
    Vec3 scaling;
    Vec3 eulerAngles;

    Mat4 localToWorld;
    bool isDirty;

    Vec3 color;
    
    const char *type;
    const size_t id;

    std::string name;
    GameStates gameState = GameStates::PreSnap;
    BallStates ballState = BallStates::PreSnap;

    float movementSpeed;

    static std::unique_ptr<DirectX::Model> model;

    void build_transformation();

    void draw_mesh();
    virtual void draw_debug();
    static bool acquire_rendering_resources();
    static void release_rendering_resources();
    static void update_model(const Mat4 &view, const Mat4 &proj);
};