#pragma once
#include "Misc/PathfindingDetails.hpp"
#include "AStarNode.h"
#include <stack>

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

	void precompute_heuristic_costs();
	float evaluate_heuristic_cost(const GridPos& start, const GridPos& destination, Heuristic type);
	float heuristic_octile(const GridPos& start, const GridPos& destination);

private:
	std::shared_ptr<AStarNode> get_min_cost_node_in_openlist();
	std::vector<std::shared_ptr<AStarNode>> get_neighbors(std::shared_ptr<AStarNode> node); // TODO: compute all heuristic
	void map_change_callback();

	void add_to_open_list(std::shared_ptr<AStarNode> node);
	void remove_from_open_list(std::shared_ptr<AStarNode> node);
	void move_from_open_to_close_list(std::shared_ptr<AStarNode> node);
	//void add_to_close_list(std::shared_ptr<AStarNode> node);
	void remove_from_close_list(std::shared_ptr<AStarNode> node);
	void generate_path(std::shared_ptr<AStarNode> finalNode, PathRequest& request);

	std::set<std::shared_ptr<AStarNode>>::const_iterator find_node_in_list(const std::set<std::shared_ptr<AStarNode>>& list, std::shared_ptr<AStarNode> node);
	//bool is_on_open_list(std::shared_ptr<AStarNode> node);
	//bool is_on_close_list(std::shared_ptr<AStarNode> node);

	std::set<std::shared_ptr<AStarNode>> openList;
	std::set<std::shared_ptr<AStarNode>> closeList;
	std::vector<std::vector<float>> heuristicCosts;
};