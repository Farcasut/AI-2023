#pragma once
#include <vector>
#include <map>
using std::map;
using std::vector;
using std::pair;

class Helper
{
public:
	inline static vector<vector<int>> final_state{};
	inline static map<int, pair<int, int>> digit_positions{};
	static vector<vector<vector<int>>> get_final_states(uint32_t rows, uint32_t columns)
	{
		vector<vector<vector<int>>> final_states;
		int cnt = 1;
		int offset = 0;
		int max_element = rows * columns;
		for (uint32_t k = 0; k < rows * columns; k++) {
			final_states.push_back(vector<vector<int>>(rows, vector<int>(columns)));
			for (uint32_t i = 0; i < rows; i++) {
				for (uint32_t j = 0; j < columns; j++) {
					final_states[k][i][j] = (cnt + offset) % max_element;
					cnt++;
				}
			}
			offset++;
		}
		return final_states;
	}
	static void set_digits()
	{
		for(int i=0; i<final_state.size(); i++)
		{
			for (int j=0; j<final_state[0].size(); j++)
			{
				digit_positions[final_state[i][j]] = std::make_pair(i, j);
			}
		}
	}
};