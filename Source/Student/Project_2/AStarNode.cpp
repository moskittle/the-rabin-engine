#include "pch.h"

#include "AStarNode.h"

AStarNode::AStarNode(GridPos _gridPos) :
	gridPos(_gridPos),
	parent(nullptr),
	givenCost(std::numeric_limits<float>::max()),
	state(AStarNodeState::NotOnList)
{
}

AStarNode::AStarNode(GridPos _gridPos, std::shared_ptr<AStarNode> _parent = nullptr,
	float _givenCost = std::numeric_limits<float>::max(), AStarNodeState _state = AStarNodeState::NotOnList) :
	gridPos(_gridPos),
	parent(_parent),
	givenCost(_givenCost),
	state(_state)
{
}


