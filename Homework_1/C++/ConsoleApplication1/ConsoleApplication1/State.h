#pragma once
#include <vector>
#include <algorithm>
#include <map>
using std::map;
using std::vector;
using std::pair;

class State
{
	vector<vector<int>> matrix;
	vector<pair<int, int>> neighbors;
	pair<int, int> zero_position;
	pair<int, int> last_movement;


	State(const State& old_state, pair<int, int> new_position)
	{
		this->matrix = old_state.matrix;
		this->last_movement = old_state.zero_position;
		this->zero_position = new_position;
		this->matrix[last_movement.first][last_movement.second] = this->matrix[new_position.first][new_position.second];
		this->matrix[new_position.first][new_position.second] = 0;
		find_neighbors();
	}
	bool validate_initial_state()
	{
		if (matrix.size()!=3)
		{
			throw std::runtime_error("The puzzle must have 3 rows!");
		}
		vector<int> reshaped_matrix;
		for(const auto& row : matrix)
		{
			if (row.size() != 3)
				throw std::runtime_error("All rows must have 3 elements!");
			reshaped_matrix.insert(reshaped_matrix.end(), row.begin(), row.end());
		}
		std::sort(reshaped_matrix.begin(), reshaped_matrix.end());
		for (int i=0 ;i<9; i++)
		{
			if (reshaped_matrix[i]!=i)
			{
				throw std::runtime_error("The matrix must contain all the number form 0 to");
			}
		}
	}
	void find_zero()
	{
		for (int i=0; i < matrix.size(); i++)
		{
			for (int j = 0; j < matrix[0].size(); j++)
			{
				if (matrix[i][j]==0)
				{
					zero_position = std::make_pair(i,j);
					return;
				}
			}
		}
	}

	void find_neighbors()
	{
		//left
		if(zero_position.second - 1 >= 0)
		{
			neighbors.push_back(std::make_pair(zero_position.first, zero_position.second - 1));
		}
		//right
		if (zero_position.second + 1 < matrix[0].size())
		{
			neighbors.push_back(std::make_pair(zero_position.first, zero_position.second + 1));
		}
		//up
		if (zero_position.first + 1 < matrix.size())
		{
			neighbors.push_back(std::make_pair(zero_position.first + 1, zero_position.second));
		}
		//down
		if (zero_position.first - 1 >= 0)
		{
			neighbors.push_back(std::make_pair(zero_position.first - 1, zero_position.second));
		}
		// remove the last move from the neighbors to not run into an infinite loop
		for (auto it = neighbors.begin(); it !=  neighbors.end(); it++)
		{
			if (*it == last_movement)
			{
				neighbors.erase(it);
				break;
			}
		}
	}
	public:
	bool is_final()
	{
		int cnt = 1;
		for (const vector<int> &row : matrix )
		{
			for (const int& cell : row)
			{
				if (cell == 0)
				{
					continue;
				}
				if (cell != cnt)
				{
					return false;
				}
				cnt++;
			}
		}
		return true;
	}

	const vector<pair<int, int>>& get_neighbors() const
	{
		return neighbors;
	}

	const vector<vector<int>>& get_matrix() const
	{
		return this->matrix;
	}

	State transition(pair<int, int>new_pos)
	{
		return State(*this, new_pos);
	}


	State(vector<vector<int>> matrix)
	{
		this->matrix = matrix;
		this->last_movement = std::make_pair(-1, -1);
		validate_initial_state();
		find_zero();
		find_neighbors();
	}
};