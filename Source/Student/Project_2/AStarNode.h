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
	AStarNode() = default;
	AStarNode(GridPos _gridPos);
	AStarNode(GridPos _gridPos, std::shared_ptr<AStarNode> _parent, float _givenCost, AStarNodeState _state);
	~AStarNode() = default;


	const GridPos& get_grid_pos() const { return gridPos; }
	const std::shared_ptr<AStarNode> get_parent() const { return parent; }
	const float get_given_cost() const { return givenCost; }

	const AStarNodeState get_state() const { return state; }
	void set_state(AStarNodeState _state) { state = _state; }

private:
	GridPos gridPos;
	std::shared_ptr<AStarNode> parent;
	float givenCost;
	// float heuristic cost;	// TODO: save heuristic cost --optimization
	AStarNodeState state;
};