#pragma once

// Include all node headers in this file

// Example Control Flow Nodes
#include "ControlFlow/C_ParallelSequencer.h"
#include "ControlFlow/C_RandomSelector.h"
#include "ControlFlow/C_Selector.h"
#include "ControlFlow/C_Sequencer.h"

#include "ControlFlow/C_SmartRouteComposer.h"

// Student Control Flow Nodes


// Example Decorator Nodes
#include "Decorator/D_Delay.h"
#include "Decorator/D_InvertedRepeater.h"
#include "Decorator/D_RepeatFourTimes.h"
#include "Decorator/D_IfGameInPlay.h"
#include "Decorator/D_IfGetBlocked.h"
#include "Decorator/D_IfMouseClickFar.h"
#include "Decorator/D_IfBallInAir.h"

// Student Decorator Nodes


// Example Leaf Nodes
#include "Leaf/L_CheckMouseClick.h"
#include "Leaf/L_Idle.h"
#include "Leaf/L_MoveToFurthestAgent.h"
#include "Leaf/L_MoveToMouseClick.h"
#include "Leaf/L_MoveToRandomPosition.h"

#include "Leaf/L_CheckQbScramble.h"
#include "Leaf/L_QbContain.h"
#include "Leaf/L_RushQb.h"
#include "Leaf/L_FootballIdle.h"
#include "Leaf/L_FinesseMove.h"
#include "Leaf/L_RouteIn.h"
#include "Leaf/L_RouteGo.h"
#include "Leaf/L_RouteOut.h"
#include "Leaf/L_RouteSlant.h"
#include "Leaf/L_ThrowBall.h"
#include "Leaf/L_ChaseBall.h"

// Student Leaf Nodes