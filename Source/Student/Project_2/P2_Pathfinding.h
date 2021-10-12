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
	void sort_list(std::vector<NodePtr>& list);
	std::vector<GridPos> generate_waypoints(NodePtr goal);

	std::vector<NodePtr> openList;
	std::vector<NodePtr> closeList;
	GridPos start;
	GridPos goal;
	Heuristic heuristicMode = Heuristic::OCTILE;
};