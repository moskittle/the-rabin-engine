#pragma once
#include "Misc/PathfindingDetails.hpp"
#include "Node.h"

using NodePtr = std::shared_ptr<Node>;

class AStarPather
{
public:
	/*
		The class should be default constructible, so you may need to define a constructor.
		If needed, you can modify the framework where the class is constructed in the
		initialize functions of ProjectTwo and ProjectThree.
	*/

	/* ************************************************** */
	// DO NOT MODIFY THESE SIGNATURES
	bool initialize();
	void shutdown();
	PathResult compute_path(PathRequest& request);
	/* ************************************************** */

	/*
		You should create whatever functions, variables, or classes you need.
		It doesn't all need to be in this header and cpp, structure it whatever way
		makes sense to you.
	*/

	std::vector<NodePtr> get_neighbors(NodePtr minNode);	// TODO:  swap tail and pre allocate --optimization
	float calc_heuristic_cost(GridPos start, GridPos end, Heuristic type = Heuristic::OCTILE);
	float calc_final_cost(NodePtr node);
	void sort_list(std::vector<NodePtr>& list);
	std::vector<GridPos> generate_waypoints(NodePtr goal);

	std::vector<GridPos> rubberband(std::vector<GridPos> waypoints);
	std::vector<Vec3> smoothing(const std::vector<Vec3>& originalWaypoints, bool isRubberbanded);
	std::vector<Vec3> get_waypoints_in_worldspace(const std::vector<GridPos>& waypoints);

	std::vector<NodePtr> openList;
	std::vector<NodePtr> closeList;
	GridPos start;
	GridPos goal;
	Heuristic heuristicMode = Heuristic::OCTILE;
	float weight = 1.0f;
	std::function<bool(NodePtr a, NodePtr b)> nodeComparor;
};