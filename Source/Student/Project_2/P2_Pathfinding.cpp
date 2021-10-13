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


#define DEBUG_COLORING(node, color, toggle) if(toggle) { terrain->set_color(node, color); }

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

	// TODO: --precompute neighbors optimization
	//Callback cb = std::bind(&AStarPather::pre_compute_neighbors, this);
	//Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

	nodeComparor = [this](NodePtr a, NodePtr b) { return a->getFinalCost(weight) > b->getFinalCost(weight); };

	return true; // return false if any errors actually occur, to stop engine initialization
}

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

	if (request.newRequest)
	{
		openList.clear();
		closeList.clear();
		start = terrain->get_grid_position(request.start);
		goal = terrain->get_grid_position(request.goal);

		heuristicMode = request.settings.heuristic;
		weight = request.settings.weight;

		DEBUG_COLORING(start, Colors::Orange, request.settings.debugColoring);
		DEBUG_COLORING(goal, Colors::Cyan, request.settings.debugColoring);

		NodePtr startNode = std::make_shared<Node>();
		NodePtr endNode = std::make_shared<Node>();
		startNode->position = start;
		endNode->position = goal;

		startNode->givenCost = 0.0f;
		startNode->heuristicCost = calc_heuristic_cost(startNode->position, endNode->position, heuristicMode);
		endNode->heuristicCost = 0.0f;

		// A* Pathfinding
		openList.push_back(startNode);
	}

	while (!openList.empty())
	{
		sort_list(openList);
		iter_swap(openList.begin(), openList.end() - 1);
		auto minNode = openList.back();
		openList.pop_back();
		closeList.push_back(minNode);
		DEBUG_COLORING(minNode->position, Colors::Yellow, request.settings.debugColoring);

		// path is found.
		if (minNode->position == goal)
		{
			// backtracing to get path
			std::vector<GridPos> waypoints = generate_waypoints(minNode);

			// rubberband
			if (request.settings.rubberBanding)
			{
				waypoints = rubberband(waypoints);
			}

			// reverse order
			std::reverse(waypoints.begin(), waypoints.end());

			// smoothing
			std::vector<Vec3> waypointsWS = get_waypoints_in_worldspace(waypoints);
			if (request.settings.smoothing)
			{
				waypointsWS = smoothing(waypointsWS, false);
			}

			// add waypoint world position to request's path
			for (auto waypointWS : waypointsWS)
			{
				request.path.push_back(waypointWS);
			}

			return PathResult::COMPLETE;
		}

		// process neighbors
		auto neighbors = get_neighbors(minNode);
		for (auto neighbor : neighbors)
		{
			auto searchComparor = [neighbor](NodePtr node) { return node->position == neighbor->position; };

			auto itrOpen = std::find_if(openList.begin(), openList.end(), searchComparor);
			bool isOnOpenList = itrOpen != openList.end();
			auto itrClose = std::find_if(closeList.begin(), closeList.end(), searchComparor);
			bool isOnCloseList = itrClose != closeList.end();

			if (!isOnOpenList && !isOnCloseList)
			{
				openList.push_back(neighbor);
				DEBUG_COLORING(neighbor->position, Colors::Blue, request.settings.debugColoring);
			}
			else
			{
				if (isOnOpenList)
				{
					auto& nodeOnlist = *itrOpen;
					if (calc_final_cost(neighbor) < calc_final_cost(nodeOnlist))
					{
						nodeOnlist->givenCost = neighbor->givenCost;
						nodeOnlist->parent = neighbor->parent;
					}
				}

				if (isOnCloseList)
				{
					auto& nodeOnlist = *itrClose;
					if (calc_final_cost(neighbor) < calc_final_cost(nodeOnlist))
					{
						nodeOnlist->givenCost = neighbor->givenCost;
						nodeOnlist->parent = neighbor->parent;
					}
				}
			}
		}

		if (request.settings.singleStep)
		{
			return PathResult::PROCESSING;
		}
	}

	return PathResult::IMPOSSIBLE;
}

std::vector<NodePtr> AStarPather::get_neighbors(NodePtr currNode)
{
	std::vector<NodePtr> result;

	GridPos currPos = currNode->position;

	int botBound = currPos.row - 1, topBound = currPos.row + 1,
		leftBound = currPos.col - 1, rightBound = currPos.col + 1;


	// check hori & vert first
	GridPos botPos(currPos.row - 1, currPos.col);
	GridPos topPos(currPos.row + 1, currPos.col);
	GridPos leftPos(currPos.row, currPos.col - 1);
	GridPos rightPos(currPos.row, currPos.col + 1);

	// narrow down the double for loop if there is a wall on left/right/bottom/top
	if (!terrain->is_valid_grid_position(topPos) || terrain->is_wall(topPos)) { topBound -= 1; }
	if (!terrain->is_valid_grid_position(botPos) || terrain->is_wall(botPos)) { botBound += 1; }
	if (!terrain->is_valid_grid_position(leftPos) || terrain->is_wall(leftPos)) { leftBound += 1; }
	if (!terrain->is_valid_grid_position(rightPos) || terrain->is_wall(rightPos)) { rightBound -= 1; }

	for (int i = botBound; i <= topBound; ++i)
	{
		for (int j = leftBound; j <= rightBound; ++j)
		{
			GridPos neighborPos(i, j);

			if (currNode->parent && neighborPos == currNode->parent->position) { continue; }

			if (neighborPos == currPos || terrain->is_wall(neighborPos)) { continue; }

			bool isDiagonal = abs(neighborPos.row - currPos.row) + abs(neighborPos.col - currPos.col) == 2;
			float givenCost = isDiagonal ? 1.41f : 1.0f;
			NodePtr newNeighbor = std::make_shared<Node>(neighborPos, currNode->givenCost + givenCost, currNode);
			newNeighbor->heuristicCost = calc_heuristic_cost(neighborPos, goal, heuristicMode);

			result.push_back(newNeighbor);
		}
	}

	return result;
}

float AStarPather::calc_heuristic_cost(GridPos start, GridPos end, Heuristic type)
{
	float xDiff = static_cast<float>(abs(start.row - end.row));
	float yDiff = static_cast<float>(abs(start.col - end.col));

	switch (type)
	{
	case Heuristic::OCTILE:
		return std::min(xDiff, yDiff) * 1.414f + (std::max(xDiff, yDiff) - std::min(xDiff, yDiff));
	case Heuristic::CHEBYSHEV:
		return std::max(xDiff, yDiff);
	case Heuristic::MANHATTAN:
		return xDiff + yDiff;
	case Heuristic::EUCLIDEAN:
		return sqrt(xDiff * xDiff + yDiff * yDiff);
	default:
		break;
	}

	assert(0);
	return -1.0f;
}

float AStarPather::calc_final_cost(NodePtr node)
{
	return node->givenCost + node->heuristicCost * weight;
}

void AStarPather::sort_list(std::vector<NodePtr>& list)
{
	std::make_heap(list.begin(), list.end(), nodeComparor);
}

std::vector<GridPos> AStarPather::generate_waypoints(NodePtr goal)
{
	std::vector<GridPos> waypoints;

	auto currNode = goal;
	while (currNode)
	{
		waypoints.push_back(currNode->position);
		currNode = currNode->parent;
	}

	return waypoints;
}

std::vector<GridPos> AStarPather::rubberband(std::vector<GridPos> waypoints)
{
	auto current = 0;
	auto next = current + 1;
	auto nextNext = next + 1;

	std::vector<GridPos> result({ waypoints[current] });
	while (nextNext < waypoints.size())
	{
		bool thereIsAWall = false;

		auto& currPt = waypoints[current];
		auto& nextPt = waypoints[next];
		auto& nextNextPt = waypoints[nextNext];

		auto leftBound = std::min(currPt.col, nextNextPt.col);
		auto rightBound = std::max(currPt.col, nextNextPt.col);
		auto botBound = std::min(currPt.row, nextNextPt.row);
		auto topBound = std::max(currPt.row, nextNextPt.row);

		for (auto i = leftBound; i <= rightBound; ++i)
		{
			for (auto j = botBound; j <= topBound; ++j)
			{
				GridPos pos(j, i);

				if (terrain->is_wall(pos))
				{
					thereIsAWall = true;
					break;
				}
			}

			if (thereIsAWall) { break; }
		}

		if (thereIsAWall)
		{
			result.push_back(waypoints[next]);
			current = next;
			next = current + 1;
			nextNext = next + 1;
		}
		else
		{
			next += 1;
			nextNext = next + 1;
		}
	}
	result.push_back(waypoints[next]);	// add the last node

	return result;
}

std::vector<Vec3> AStarPather::smoothing(const std::vector<Vec3>& originalWaypoints, bool isRubberbanded = false)
{
	if (originalWaypoints.size() < 4)
	{
		return originalWaypoints;
	}

	std::vector<Vec3> result;

	if (isRubberbanded)
	{
		// add points
	}


	// special case for first one
	auto firstNode = originalWaypoints.begin();
	auto first = *firstNode, second = *firstNode, third = *(firstNode + 1), fourth = *(firstNode + 2);

	auto firstExtra = Vec3::CatmullRom(first, second, third, fourth, 0.25f);
	auto secondExtra = Vec3::CatmullRom(first, second, third, fourth, 0.5f);
	auto thirdExtra = Vec3::CatmullRom(first, second, third, fourth, 0.75f);

	result.push_back(second);
	result.push_back(firstExtra);
	result.push_back(secondExtra);
	result.push_back(thirdExtra);

	// skip the first and last one. these two are special cases
	for (auto current = originalWaypoints.begin() + 1; current != originalWaypoints.end() - 3; ++current)
	{
		auto& first = *current, & second = *(current + 1), & third = *(current + 2), & fourth = *(current + 3);

		auto firstExtra = Vec3::CatmullRom(first, second, third, fourth, 0.25f);
		auto secondExtra = Vec3::CatmullRom(first, second, third, fourth, 0.5f);
		auto thirdExtra = Vec3::CatmullRom(first, second, third, fourth, 0.75f);

		result.push_back(second);

		result.push_back(firstExtra);
		result.push_back(secondExtra);
		result.push_back(thirdExtra);

	}

	firstNode = originalWaypoints.end() - 3;
	first = *firstNode, second = *firstNode, third = *(firstNode + 1), fourth = *(firstNode + 2);

	firstExtra = Vec3::CatmullRom(first, second, third, fourth, 0.25f);
	secondExtra = Vec3::CatmullRom(first, second, third, fourth, 0.5f);
	thirdExtra = Vec3::CatmullRom(first, second, third, fourth, 0.75f);

	result.push_back(firstExtra);
	result.push_back(secondExtra);
	result.push_back(thirdExtra);
	result.push_back(originalWaypoints[originalWaypoints.size() - 1]);

	return result;
}

std::vector<Vec3> AStarPather::get_waypoints_in_worldspace(const std::vector<GridPos>& waypoints)
{
	std::vector<Vec3> waypointsWS;

	for (auto& waypoint : waypoints)
	{
		waypointsWS.push_back(terrain->get_world_position(waypoint));
	}

	return waypointsWS;
}
