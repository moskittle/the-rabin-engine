#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"


#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
	return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
	return false;
}

bool ProjectTwo::implemented_jps_plus()
{
	return false;
}
#pragma endregion

bool AStarPather::initialize()
{
	// handle any one-time setup requirements you have

	/*
		If you want to do any map-preprocessing, you'll need to listen
		for the map change message.  It'll look something like this:

		Callback cb = std::bind(&AStarPather::your_function_name, this);
		Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

		There are other alternatives to using std::bind, so feel free to mix it up.
		Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
		object that std::function can wrap will suffice.
	*/

	Callback mapChangeCb = std::bind(&AStarPather::map_change_callback, this);
	Messenger::listen_for_message(Messages::MAP_CHANGE, mapChangeCb);

	return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::map_change_callback()
{
	printf("Precompute heuristic costs.");

	std::vector<std::vector<int>> newHeuristicCosts(terrain->get_map_height(), std::vector<int>(terrain->get_map_width(), -1));
	heuristicCosts.clear();

	//precompute_heuristic_costs();

}

void AStarPather::add_to_open_list(std::shared_ptr<AStarNode> node)
{
	openList.insert(node);
	node->set_state(AStarNodeState::OpenList);
	terrain->set_color(node->get_grid_pos(), Colors::Blue);
}

void AStarPather::remove_from_open_list(std::shared_ptr<AStarNode> node)
{
	openList.erase(node);
	node->set_state(AStarNodeState::NotOnList);
	terrain->set_color(node->get_grid_pos(), Colors::White);
}

void AStarPather::move_from_open_to_close_list(std::shared_ptr<AStarNode> node)
{
	openList.erase(node);
	closeList.insert(node);
	node->set_state(AStarNodeState::CloseList);
	terrain->set_color(node->get_grid_pos(), Colors::Yellow);
}

//void AStarPather::add_to_close_list(std::shared_ptr<AStarNode> node)
//{
//	closeList.insert(node);
//	node->set_state(AStarNodeState::CloseList);
//	terrain->set_color(node->get_grid_pos(), Colors::Cyan);
//}

void AStarPather::remove_from_close_list(std::shared_ptr<AStarNode> node)
{
	closeList.erase(node);
	node->set_state(AStarNodeState::NotOnList);
	terrain->set_color(node->get_grid_pos(), Colors::White);
}

void AStarPather::generate_path(std::shared_ptr<AStarNode> finalNode, PathRequest& request)
{
	auto curr = finalNode;
	std::stack<std::shared_ptr<AStarNode>> pathNodes({ curr });
	while (curr->get_parent())
	{
		auto parent = curr->get_parent();
		auto itr = find_node_in_list(closeList, parent);
		assert(*itr);

		curr = *itr;
		pathNodes.push(curr);
	}


	while (!pathNodes.empty())
	{
		auto curr = pathNodes.top();
		pathNodes.pop();

		request.path.push_back(terrain->get_world_position(curr->get_grid_pos()));
	}
}

std::set<std::shared_ptr<AStarNode>>::const_iterator AStarPather::find_node_in_list(const std::set<std::shared_ptr<AStarNode>>& list, std::shared_ptr<AStarNode> node)
{
	auto itr = std::find_if(list.begin(), list.end(),
		[node](std::shared_ptr<AStarNode> n) { return node->get_grid_pos() == n->get_grid_pos(); });

	return itr;
}

//bool AStarPather::is_on_open_list(std::shared_ptr<AStarNode> node)
//{
//	bool result = std::find_if(openList.begin(), openList.end(),
//		[node](std::shared_ptr<AStarNode> n) { return node->get_grid_pos() == n->get_grid_pos(); }) != openList.end();
//
//	return result;
//}
//
//bool AStarPather::is_on_close_list(std::shared_ptr<AStarNode> node)
//{
//	bool result = std::find_if(closeList.begin(), closeList.end(),
//		[node](std::shared_ptr<AStarNode> n) { return node->get_grid_pos() == n->get_grid_pos(); }) != closeList.end();
//
//	return result;
//}


void AStarPather::shutdown()
{
	/*
		Free any dynamically allocated memory or any other general house-
		keeping you need to do during shutdown.
	*/
}

PathResult AStarPather::compute_path(PathRequest& request)
{
	/*
		This is where you handle pathing requests, each request has several fields:

		start/goal - start and goal world positions
		path - where you will build the path upon completion, path should be
			start to goal, not goal to start
		heuristic - which heuristic calculation to use
		weight - the heuristic weight to be applied
		newRequest - whether this is the first request for this path, should generally
			be true, unless single step is on

		smoothing - whether to apply smoothing to the path
		rubberBanding - whether to apply rubber banding
		singleStep - whether to perform only a single A* step
		debugColoring - whether to color the grid based on the A* state:
			closed list nodes - yellow
			open list nodes - blue

			use terrain->set_color(row, col, Colors::YourColor);
			also it can be helpful to temporarily use other colors for specific states
			when you are testing your algorithms

		method - which algorithm to use: A*, Floyd-Warshall, JPS+, or goal bounding,
			will be A* generally, unless you implement extra credit features

		The return values are:
			PROCESSING - a path hasn't been found yet, should only be returned in
				single step mode until a path is found
			COMPLETE - a path to the goal was found and has been built in request.path
			IMPOSSIBLE - a path from start to goal does not exist, do not add start position to path
	*/

	// WRITE YOUR CODE HERE

	if (request.newRequest)
	{
		openList.clear();
		closeList.clear();
	}

	GridPos start = terrain->get_grid_position(request.start);
	GridPos goal = terrain->get_grid_position(request.goal);

	auto heuristicCost = evaluate_heuristic_cost(start, goal, Heuristic::OCTILE);
	auto startNode = std::make_shared<AStarNode>(start, nullptr, 0.0f, AStarNodeState::NotOnList);

	openList.insert(startNode);
	while (!openList.empty())
	{
		auto minCostNode = get_min_cost_node_in_openlist();
		move_from_open_to_close_list(minCostNode);

		if (minCostNode->get_grid_pos() == goal)
		{
			// generate path
			printf("found path\n");
			generate_path(minCostNode, request);


			return PathResult::COMPLETE;
		}

		// get current node's neighbors
		auto neighbors = get_neighbors(minCostNode);

		for (auto neighbor : neighbors)
		{
			// add to open list if not on any list
			bool neighborOnOpenList = find_node_in_list(openList, neighbor) != openList.end();
			bool neighborOnCloseList = find_node_in_list(closeList, neighbor) != closeList.end();
			if (!neighborOnOpenList && !neighborOnCloseList)
			{
				add_to_open_list(neighbor);
			}
			else
			{
				auto neighborFinalCost = neighbor->get_given_cost() + evaluate_heuristic_cost(neighbor->get_grid_pos(), goal, Heuristic::OCTILE);


				std::shared_ptr<AStarNode> neighborOnOpenList, neighborOnCloseList;
				if (!openList.empty())
				{
					auto targetItr = std::find_if(openList.begin(), openList.end(), [neighbor](std::shared_ptr<AStarNode> n) { return n->get_grid_pos() == neighbor->get_grid_pos(); });

					if (targetItr != openList.end())
					{
						neighborOnOpenList = *targetItr;
						auto originalFinalCost = neighborOnOpenList->get_given_cost() + evaluate_heuristic_cost(neighborOnOpenList->get_grid_pos(), goal, Heuristic::OCTILE);
						if (neighborFinalCost < originalFinalCost)
						{
							remove_from_open_list(neighborOnOpenList);
							add_to_open_list(neighbor);
						}
					}
				}

				if (!closeList.empty())
				{
					auto targetIter = std::find_if(closeList.begin(), closeList.end(), [neighbor](std::shared_ptr<AStarNode> n) { return n->get_grid_pos() == neighbor->get_grid_pos(); });

					if (targetIter != closeList.end())
					{
						neighborOnCloseList = *targetIter;
						auto originalFinalCost = neighborOnCloseList->get_given_cost() + evaluate_heuristic_cost(neighborOnCloseList->get_grid_pos(), goal, Heuristic::OCTILE);
						if (neighborFinalCost < originalFinalCost)
						{
							//remove_from_close_list(neighborOnCloseList);
						}
					}
				}

				//printf("%f, %f", neighborFinalCost, neighborOnOpenList->get_given_cost());

			}
		}
	}

	terrain->set_color(start, Colors::Coral);
	terrain->set_color(goal, Colors::Cyan);
	request.path.push_back(request.start);
	request.path.push_back(request.goal);




	// TODO: -- more features
	if (request.settings.rubberBanding) {}
	if (request.settings.smoothing) {}








	return PathResult::COMPLETE;
}

void AStarPather::precompute_heuristic_costs()
{
	// TODO: -- use try catch to check if terrain is null

	auto width = terrain->get_map_width();
	auto height = terrain->get_map_height();

	//for (int i = 0; i < height; ++i)
	//{
	//	for (int j = 0; j < width; ++j)
	//	{
	//		GridPos currPos(i, j);
	//		heuristicCosts[i][j] = evaluate_heuristic_cost(currPos, destination);
	//	}
	//}
}

float AStarPather::evaluate_heuristic_cost(const GridPos& start, const GridPos& destination, Heuristic type)
{
	switch (type)
	{
	case Heuristic::OCTILE:
		break;
	case Heuristic::CHEBYSHEV:
		break;
	case Heuristic::MANHATTAN:
		break;
	case Heuristic::EUCLIDEAN:
		break;
	case Heuristic::NUM_ENTRIES:
		break;
	default:
		break;
	}

	int xDiff = destination.row - start.row;
	int yDiff = destination.col - start.col;
	float squaredSum = static_cast<float>(xDiff * xDiff + yDiff * yDiff);
	float heuristicCost = sqrt(squaredSum);

	return heuristicCost;
}

std::shared_ptr<AStarNode> AStarPather::get_min_cost_node_in_openlist()
{
	if (!openList.empty())
	{
		return *openList.begin();
	}

	return nullptr;
}

std::vector<std::shared_ptr<AStarNode>> AStarPather::get_neighbors(std::shared_ptr<AStarNode> node)
{
	auto currPos = node->get_grid_pos();
	std::vector<std::shared_ptr<AStarNode>> neighbors;
	neighbors.reserve(8);

	// find 8 neighbors
	int leftBound = currPos.row - 1;
	int rightBound = currPos.row + 1;
	int bottomBound = currPos.col - 1;
	int upperBound = currPos.col + 1;

	for (int i = bottomBound; i <= upperBound; ++i)
	{
		if (i < 0 || i >= terrain->get_map_height()) { continue; }

		for (int j = leftBound; j <= rightBound; ++j)
		{
			// 1. node check
			if (j < 0 || j >= terrain->get_map_width()) { continue; }

			GridPos neighborPos(i, j);
			auto parentNode = node->get_parent();

			if (parentNode)
			{
				auto parentPos = parentNode->get_grid_pos();
				bool isParent = neighborPos == parentPos;
				if (isParent) { continue; }
			}

			bool isSelf = currPos == neighborPos;
			bool isWall = terrain->is_wall(i, j);
			if (isWall || isSelf) { continue; }

			// 2. logic
			auto neighbor = std::make_shared<AStarNode>(neighborPos, node, node->get_given_cost() + 1.0f, AStarNodeState::NotOnList);

			neighbors.push_back(neighbor);
		}
	}

	return neighbors;
}


