#pragma once

enum class GameStates
{
    PreSnap,
    InPlay,
    PostPlay
};

enum class BallStates
{
    PreSnap,
    InQBsHand,
    InAir,
    InWRsHand,
    OnGround
};