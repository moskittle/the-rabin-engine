#pragma once

// TODO: possible optimization
enum class AStarNodeState
{
    NotOnList,
    OpenList,
    CloseList
};

class AStarNode
{
public:
    AStarNode(GridPos _gridPos, std::shared_ptr<AStarNode> _parent, float _finalCost = std::numeric_limits<float>::max(),
              AStarNodeState _state = AStarNodeState::NotOnList);
    ~AStarNode() = default;

    const GridPos& getGridPos() const { return gridPos; };
    const std::shared_ptr<AStarNode> getParent() const { return parent; };
    const float getfinalCost() const { return finalCost; };
    const AStarNodeState getState() const { return state; };


private:
    GridPos gridPos;
    std::shared_ptr<AStarNode> parent;
    float finalCost;
    AStarNodeState state;
};