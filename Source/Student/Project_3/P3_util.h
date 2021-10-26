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

	float CalcDirectInfluence(MapLayer<float>& layer, float decay, GridPos directNeighbor, bool& flag)
	{
		if (terrain->is_valid_grid_position(directNeighbor))
		{
			if (terrain->is_wall(directNeighbor))
			{
				flag = true;
				return 0.0f;
			}

			float distance = 1.0f;
			return layer.get_value(directNeighbor) * expf(-1.0f * distance * decay);
		}

		return 0.0f;
	}

	float CalcDiagonalInfluence(MapLayer<float>& layer, float decay, GridPos diagonalNeighbor, bool flag)
	{
		if (terrain->is_valid_grid_position(diagonalNeighbor))
		{
			if (terrain->is_wall(diagonalNeighbor) || flag)
			{
				return 0.0f;
			}

			float distance = 1.414f; // sqrt of 2
			return layer.get_value(diagonalNeighbor) * expf(-1.0f * distance * decay);
		}

		return 0.0f;
	}


}