#pragma once

namespace P3_util
{

	// Check top, bot, left, right neighbors. (No diagonal neighbors)
	void CheckDirectNeighbor(GridPos directNeighbor, bool& flag, std::vector<std::vector<float>>& visibility)
	{
		if (terrain->is_valid_grid_position(directNeighbor))
		{
			if (terrain->is_wall(directNeighbor))
			{
				flag = true;
			}
			else
			{
				if (fabs(visibility[directNeighbor.row][directNeighbor.col] - 1.0f) < 0.001f) { return; }
				visibility[directNeighbor.row][directNeighbor.col] = 0.5f;
			}
		}
	}

	void CheckDiagonalNeighbor(GridPos diagonalNeighbor, bool flag, std::vector<std::vector<float>>& visibility)
	{
		if (terrain->is_valid_grid_position(diagonalNeighbor))
		{
			if (terrain->is_wall(diagonalNeighbor)) { return; }

			if (flag) { return; }

			if (fabs(visibility[diagonalNeighbor.row][diagonalNeighbor.col] - 1.0f) < 0.001f) { return; }
			visibility[diagonalNeighbor.row][diagonalNeighbor.col] = 0.5f;
		}
	}


}