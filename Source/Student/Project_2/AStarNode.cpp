#include "pch.h"

#include "AStarNode.h"

AStarNode::AStarNode(GridPos _gridPos, std::shared_ptr<AStarNode> _parent,
                     float _finalCost, AStarNodeState _state) :
    gridPos(_gridPos),
    parent(_parent),
    finalCost(_finalCost),
    state(_state)
{
}
