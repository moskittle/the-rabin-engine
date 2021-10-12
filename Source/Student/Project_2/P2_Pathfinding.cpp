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

	// WRITE YOUR CODE HERE
	GridPos start = terrain->get_grid_position(request.start);
	GridPos goal = terrain->get_grid_position(request.goal);

	NodePtr startNode = std::make_shared<Node>();
	NodePtr endNode = std::make_shared<Node>();
	startNode->position = start;
	endNode->position = goal;
	startNode->heuristicCost = calc_heuristic_cost(Heuristic::MANHATTAN, startNode->position, endNode->position);
	endNode->heuristicCost = 0.0f;


	// A* Pathfinding
	openList.push(startNode);
	while (!openList.empty())
	{
		auto minNode = openList.top();
		openList.pop();

		// path is found.
		if (minNode->position == goal)
		{

			return PathResult::COMPLETE;
		}

		// process neighbors
		auto neighbors = get_neighbors(minNode);

		printf("neighbor size: %d\n", (int)neighbors.size());

	}




	// Just sample code, safe to delete


	terrain->set_color(start, Colors::Orange);
	terrain->set_color(goal, Colors::Cyan);

	request.path.push_back(request.start);
	request.path.push_back(request.goal);

	return PathResult::COMPLETE;
}

std::vector<NodePtr> AStarPather::get_neighbors(NodePtr currNode)
{
	std::vector<NodePtr> result;

	GridPos currPos = currNode->position;

	int botBound = currPos.col - 1, upBound = currPos.col + 1,
		leftBound = currPos.row - 1, rightBound = currPos.row + 1;


	// check hori & vert first
	GridPos upPos(currPos.row, currPos.col + 1);
	GridPos botPos(currPos.row, currPos.col - 1);
	GridPos leftPos(currPos.row, currPos.row - 1);
	GridPos rightPos(currPos.row, currPos.row + 1);

	if (!terrain->is_valid_grid_position(upPos) || terrain->is_wall(upPos)) { upBound -= 1; }
	if (!terrain->is_valid_grid_position(botPos) || terrain->is_wall(botPos)) { botBound += 1; }
	if (!terrain->is_valid_grid_position(leftPos) || terrain->is_wall(leftPos)) { leftBound += 1; }
	if (!terrain->is_valid_grid_position(rightPos) || terrain->is_wall(rightPos)) { rightBound -= 1; }

	for (int i = botBound; i <= upBound; ++i)
	{
		for (int j = leftBound; j <= rightBound; ++j)
		{
			GridPos neighborPos(i, j);

			if (neighborPos == currPos || terrain->is_wall(neighborPos)) { continue; }

			bool isDiagonal = abs(neighborPos.row - currPos.row) + abs(neighborPos.col - currPos.col) == 2;
			float givenCost = isDiagonal ? 1.41f : 1.0f;
			NodePtr newNeighbor = std::make_shared<Node>(neighborPos, currNode->givenCost + givenCost, currNode);

			result.push_back(newNeighbor);
		}
	}

	return result;
}

float AStarPather::calc_heuristic_cost(Heuristic type, GridPos start, GridPos end)
{
	float xDiff = static_cast<float>(abs(start.row - end.row));
	float yDiff = static_cast<float>(abs(start.col - end.col));

	switch (type)
	{
	case Heuristic::OCTILE:
		return std::min(xDiff, yDiff) + (std::max(xDiff, yDiff) - std::min(xDiff, yDiff)) * 1.41f;
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
