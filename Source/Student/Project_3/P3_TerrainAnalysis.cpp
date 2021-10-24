#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include "P3_util.h"

#include <iostream>

bool ProjectThree::implemented_fog_of_war() const // extra credit
{
	return false;
}

float distance_to_closest_wall(int row, int col)
{
	/*
		Check the euclidean distance from the given cell to every other wall cell,
		with cells outside the map bounds treated as walls, and return the smallest
		distance.  Make use of the is_valid_grid_position and is_wall member
		functions in the global terrain to determine if a cell is within map bounds
		and a wall, respectively.
	*/

	// WRITE YOUR CODE HERE
	float closestDistance = std::numeric_limits<float>::max();

	int range = 1;
	int botBound = row - range, topBound = row + range;
	int leftBound = col - range, rightBound = col + range;
	int mapHeight = terrain->get_map_height();
	int mapWidth = terrain->get_map_width();
	bool wallFound = false;

	while ((botBound >= 0 || topBound < mapHeight) && (leftBound >= 0 || rightBound < mapWidth) && !wallFound)
	{
		for (int i = botBound; i <= topBound; ++i)
		{
			for (int j = leftBound; j <= rightBound; ++j)
			{
				GridPos currPos = GridPos(i, j);

				// skip itself
				if (currPos == GridPos(row, col)) { continue; }

				// get distance if it is a wall or out of bounds
				if (!terrain->is_valid_grid_position(currPos) || terrain->is_wall(currPos))
				{
					int xDiff = col - j;
					int yDiff = row - i;
					float currentDistance = (float)sqrt((float)(xDiff * xDiff + yDiff * yDiff));

					closestDistance = std::min(currentDistance, closestDistance);
					wallFound = true;
				}
			}
		}

		range++;
		botBound = row - range, topBound = row + range;
		leftBound = col - range, rightBound = col + range;
	}

	return closestDistance; // REPLACE THIS
}

bool is_clear_path(int row0, int col0, int row1, int col1)
{
	/*
		Two cells (row0, col0) and (row1, col1) are visible to each other if a line
		between their centerpoints doesn't intersect the four boundary lines of every
		//wall cell.  You should puff out the four boundary lines by a very tiny amount
		so that a diagonal line passing by the corner will intersect it.  Make use of the
		line_intersect helper function for the intersection test and the is_wall member
		function in the global terrain to determine if a cell is a wall or not.
	*/

	// WRITE YOUR CODE HERE
	int botBound = std::min(row0, row1), topBound = std::max(row0, row1);
	int leftBound = std::min(col0, col1), rightBound = std::max(col0, col1);

	// 1. get a line between center points
	Vec2 startPoint = Vec2((float)row0, (float)col0);
	Vec2 endPoint = Vec2((float)row1, (float)col1);

	for (int i = botBound; i <= topBound; ++i)
	{
		for (int j = leftBound; j <= rightBound; ++j)
		{
			GridPos currPos = GridPos(i, j);

			if (terrain->is_wall(currPos))
			{
				// 4 boundaries
				Vec2 centerPos = Vec2((float)currPos.row, (float)currPos.col);
				Vec2 p0 = centerPos + Vec2(-1.0f, -1.0f) * 0.5f;
				Vec2 p1 = centerPos + Vec2(-1.0f, 1.0f) * 0.5f;
				Vec2 p2 = centerPos + Vec2(1.0f, 1.0f) * 0.5f;
				Vec2 p3 = centerPos + Vec2(1.0f, -1.0f) * 0.5f;

				// check intersection with four lines
				if (line_intersect(startPoint, endPoint, p0, p1))
				{
					return false;
				}

				if (line_intersect(startPoint, endPoint, p1, p2))
				{
					return false;
				}

				if (line_intersect(startPoint, endPoint, p2, p3))
				{
					return false;
				}

				if (line_intersect(startPoint, endPoint, p3, p0))
				{
					return false;
				}

			}
		}
	}

	return true; // REPLACE THIS
}

void analyze_openness(MapLayer<float>& layer)
{
	/*
		Mark every cell in the given layer with the value 1 / (d * d),
		where d is the distance to the closest wall or edge.  Make use of the
		distance_to_closest_wall helper function.  Walls should not be marked.
	*/

	// WRITE YOUR CODE HERE
	int width = terrain->get_map_width();
	int height = terrain->get_map_height();

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			GridPos currentPos = GridPos(i, j);

			if (terrain->is_wall(currentPos)) { continue; }

			// fill color
			float distance = distance_to_closest_wall(i, j);
			float openness = 1.0f / (distance * distance);
			layer.set_value(i, j, openness);
		}
	}
}

void analyze_visibility(MapLayer<float>& layer)
{
	/*
		Mark every cell in the given layer with the number of cells that
		are visible to it, divided by 160 (a magic number that looks good).  Make sure
		to cap the value at 1.0 as well.

		Two cells are visible to each other if a line between their centerpoints doesn't
		intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
		helper function.
	*/

	int width = terrain->get_map_width();
	int height = terrain->get_map_height();

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			GridPos currentCell = GridPos(i, j);

			if (terrain->is_wall(currentCell)) { continue; }

			// check this cell with all other cells
			int visibleCount = 0;
			for (int m = 0; m < height; ++m)
			{
				for (int n = 0; n < width; ++n)
				{
					GridPos testCell = GridPos(m, n);

					if (terrain->is_wall(testCell) || currentCell == testCell) { continue; }

					if (is_clear_path(i, j, m, n))
					{
						visibleCount++;
					}
				}
			}

			float visibility = (float)visibleCount / 160.0f;
			visibility = std::min(visibility, 1.0f);
			layer.set_value(i, j, visibility);
		}
	}
}

void analyze_visible_to_cell(MapLayer<float>& layer, int row, int col)
{
	/*
		For every cell in the given layer mark it with 1.0
		if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
		or 0.0 otherwise.

		Two cells are visible to each other if a line between their centerpoints doesn't
		intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
		helper function.
	*/

	// WRITE YOUR CODE HERE
	GridPos currCell = GridPos(row, col);

	int width = terrain->get_map_width();
	int height = terrain->get_map_height();

	std::vector<std::vector<float>> visibility(height, std::vector<float>(width, 0.0f));

	// 1. calculate fully visible cells
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			GridPos cell = GridPos(i, j);

			if (terrain->is_wall(cell)) { continue; }

			visibility[i][j] = is_clear_path(currCell.row, currCell.col, cell.row, cell.col) ? 1.0f : 0.0f;
		}
	}

	// 2. calculate half visible cells
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			GridPos cell = GridPos(i, j);
			if (terrain->is_wall(cell)) { continue; }

			// 2.1 propagate half visibility to neighbor if the cell is visible
			if (fabs(visibility[i][j] - 1.0f) < 0.001f)
			{
				// turn non-visible neighbors to visible cells
				int bot = i - 1, top = i + 1, left = j - 1, right = j + 1;
				GridPos botNeighbor(bot, j), topNeighbor(top, j), leftNeighbor(i, left), rightNeighbor(i, right);
				bool botWallFlag = false, topWallFlag = false, leftWallFlag = false, rightWallFlag = false;

				// 2.1.1 check direct neighbors (non-diagonal)
				P3_util::CheckDirectNeighbor(botNeighbor, botWallFlag, visibility);
				P3_util::CheckDirectNeighbor(topNeighbor, topWallFlag, visibility);
				P3_util::CheckDirectNeighbor(leftNeighbor, leftWallFlag, visibility);
				P3_util::CheckDirectNeighbor(rightNeighbor, rightWallFlag, visibility);

				// 2.1.2 check diagonal neighbors
				GridPos botLeft(bot, left), botRight(bot, right), topLeft(top, left), topRight(top, right);

				P3_util::CheckDiagonalNeighbor(botLeft, botWallFlag || leftWallFlag, visibility);
				P3_util::CheckDiagonalNeighbor(botRight, botWallFlag || rightWallFlag, visibility);
				P3_util::CheckDiagonalNeighbor(topLeft, topWallFlag || leftWallFlag, visibility);
				P3_util::CheckDiagonalNeighbor(topRight, topWallFlag || rightWallFlag, visibility);
			}
		}
	}

	// 3. fill color
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			GridPos cell = GridPos(i, j);

			if (terrain->is_wall(cell)) { continue; }

			layer.set_value(i, j, visibility[i][j]);
		}
	}

}

void analyze_agent_vision(MapLayer<float>& layer, const Agent* agent)
{
	/*
		For every cell in the given layer that is visible to the given agent,
		mark it as 1.0, otherwise don't change the cell's current value.

		You must consider the direction the agent is facing.  All of the agent data is
		in three dimensions, but to simplify you should operate in two dimensions, the XZ plane.

		Take the dot product between the view vector and the vector from the agent to the cell,
		both normalized, and compare the cosines directly instead of taking the arccosine to
		avoid introducing floating-point inaccuracy (larger cosine means smaller angle).

		Give the agent a field of view slighter larger than 180 degrees.

		Two cells are visible to each other if a line between their centerpoints doesn't
		intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
		helper function.
	*/

	// WRITE YOUR CODE HERE
}

void propagate_solo_occupancy(MapLayer<float>& layer, float decay, float growth)
{
	/*
		For every cell in the given layer:

			1) Get the value of each neighbor and apply decay factor
			2) Keep the highest value from step 1
			3) Linearly interpolate from the cell's current value to the value from step 2
			   with the growing factor as a coefficient.  Make use of the lerp helper function.
			4) Store the value from step 3 in a temporary layer.
			   A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

		After every cell has been processed into the temporary layer, write the temporary layer into
		the given layer;
	*/

	// WRITE YOUR CODE HERE
}

void propagate_dual_occupancy(MapLayer<float>& layer, float decay, float growth)
{
	/*
		Similar to the solo version, but the values range from -1.0 to 1.0, instead of 0.0 to 1.0

		For every cell in the given layer:

		1) Get the value of each neighbor and apply decay factor
		2) Keep the highest ABSOLUTE value from step 1
		3) Linearly interpolate from the cell's current value to the value from step 2
		   with the growing factor as a coefficient.  Make use of the lerp helper function.
		4) Store the value from step 3 in a temporary layer.
		   A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

		After every cell has been processed into the temporary layer, write the temporary layer into
		the given layer;
	*/

	// WRITE YOUR CODE HERE
}

void normalize_solo_occupancy(MapLayer<float>& layer)
{
	/*
		Determine the maximum value in the given layer, and then divide the value
		for every cell in the layer by that amount.  This will keep the values in the
		range of [0, 1].  Negative values should be left unmodified.
	*/

	// WRITE YOUR CODE HERE
}

void normalize_dual_occupancy(MapLayer<float>& layer)
{
	/*
		Similar to the solo version, but you need to track greatest positive value AND
		the least (furthest from 0) negative value.

		For every cell in the given layer, if the value is currently positive divide it by the
		greatest positive value, or if the value is negative divide it by -1.0 * the least negative value
		(so that it remains a negative number).  This will keep the values in the range of [-1, 1].
	*/

	// WRITE YOUR CODE HERE
}

void enemy_field_of_view(MapLayer<float>& layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent* enemy)
{
	/*
		First, clear out the old values in the map layer by setting any negative value to 0.
		Then, for every cell in the layer that is within the field of view cone, from the
		enemy agent, mark it with the occupancy value.  Take the dot product between the view
		vector and the vector from the agent to the cell, both normalized, and compare the
		cosines directly instead of taking the arccosine to avoid introducing floating-point
		inaccuracy (larger cosine means smaller angle).

		If the tile is close enough to the enemy (less than closeDistance),
		you only check if it's visible to enemy.  Make use of the is_clear_path
		helper function.  Otherwise, you must consider the direction the enemy is facing too.
		This creates a radius around the enemy that the player can be detected within, as well
		as a fov cone.
	*/

	// WRITE YOUR CODE HERE
}

bool enemy_find_player(MapLayer<float>& layer, AStarAgent* enemy, Agent* player)
{
	/*
		Check if the player's current tile has a negative value, ie in the fov cone
		or within a detection radius.
	*/

	const auto& playerWorldPos = player->get_position();

	const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

	// verify a valid position was returned
	if (terrain->is_valid_grid_position(playerGridPos) == true)
	{
		if (layer.get_value(playerGridPos) < 0.0f)
		{
			return true;
		}
	}

	// player isn't in the detection radius or fov cone, OR somehow off the map
	return false;
}

bool enemy_seek_player(MapLayer<float>& layer, AStarAgent* enemy)
{
	/*
		Attempt to find a cell with the highest nonzero value (normalization may
		not produce exactly 1.0 due to floating point error), and then set it as
		the new target, using enemy->path_to.

		If there are multiple cells with the same highest value, then pick the
		cell closest to the enemy.

		Return whether a target cell was found.
	*/

	// WRITE YOUR CODE HERE

	return false; // REPLACE THIS
}
