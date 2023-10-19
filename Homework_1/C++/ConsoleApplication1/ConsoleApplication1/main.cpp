#include <iostream>
#include <stack>
#include <set>
#include <queue>
#include <map>
#include "Helper.h"
#include "State.h"
using std::map;
using std::stack;
using std::set;
using std::queue;
using std::priority_queue;

void print_solution(const State& state, const uint64_t& steps )
{
	std::cout << "You found a solution!\n";
	std::cout << "No. of traversed states: " << steps << "\n";
	for (const auto& row : state.get_matrix())
	{
		for(const auto& cell :  row)
		{
			std::cout << cell<<" ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

double error(const vector<double>& elements)
{
	double error = 0;
	for (const auto& element : elements)
	{
		error += pow(element, 2);
	}
	return sqrt(error / elements.size());
}

double hamming_distance(const vector<vector<int>>& current_state, const vector<vector<int>>& final_state)
{
	double differences = 0;
	for (int i = 0; i < current_state.size(); i++)
	{
		for (int j = 0; j < current_state[0].size(); j++)
		{
			if (current_state[i][j] != final_state[i][j])
			{
				differences++;
			}
		}
	}
	return differences;
}

double euclidean_distance(const vector<vector<int>>& current_state, const vector<vector<int>>& final_state)
{
	vector<double> distances{};
	for (int i = 0; i < current_state.size(); i++)
	{
		for (int j = 0; j < current_state[0].size(); j++)
		{
			if (current_state[i][j] != final_state[i][j])
			{
				const auto& [a, b] = Helper::digit_positions[final_state[i][j]];

				distances.push_back(sqrt(pow(a - i, 2) + pow(b - j, 2)));
			}
		}
	}
	return error(distances);
}

double manhattan_distance(const vector<vector<int>>& current_state, const vector<vector<int>>& final_state)
{
	vector<double> distances{};
	for (int i = 0; i < current_state.size(); i++)
	{
		for (int j = 0; j < current_state[0].size(); j++)
		{
			if (current_state[i][j] != final_state[i][j])
			{
				const auto& [a, b] = Helper::digit_positions[final_state[i][j]];

				distances.push_back(abs(a - i) + abs(b - i));
			}
		}
	}
	return error(distances);
}

void dfs(const vector<vector<int>>& matrix)
{
	int states = 0;
	State initial_state(matrix);
	stack<State> stack;
	stack.push(initial_state);
	set<vector<vector<int>>> visited_states;

	while (!stack.empty())
	{
		State current_state = stack.top();
		states++;
		stack.pop();
		if (current_state.is_final())
		{
			print_solution(current_state, states);
			return;
		}
		visited_states.insert(current_state.get_matrix());

		for (const auto& neighbor : current_state.get_neighbors())
		{
			State new_state = current_state.transition(neighbor);
			if (!visited_states.count(new_state.get_matrix()))
			{
				stack.push(new_state);
			}
		}
	}
}

void bfs(const vector<vector<int>>& matrix)
{
	int states = 0;
	State initial_state(matrix);
	queue<State> stack;
	stack.push(initial_state);
	set<vector<vector<int>>> visited_states;

	while (!stack.empty())
	{
		State current_state = stack.front();
		stack.pop();
		states++;
		if (current_state.is_final())
		{
			print_solution(current_state, states);
			return;
		}

		for (const auto& neighbor : current_state.get_neighbors())
		{
			State new_state = current_state.transition(neighbor);
			if (!visited_states.count(new_state.get_matrix()))
			{
				visited_states.insert(current_state.get_matrix());
				stack.push(new_state);
			}
		}
	}
}



bool dfs(const vector<vector<int>>& matrix, uint32_t max_depth)
{
	int states = 0;
	State initial_state(matrix);
	stack<pair<State, uint32_t>> stack;
	stack.push(std::make_pair(initial_state, 0));
	set<vector<vector<int>>> visited_states;

	while (!stack.empty())
	{
		auto current_pair = stack.top();
		stack.pop();
		State current_state = current_pair.first;
		uint32_t current_depth = current_pair.second;

		states++;

		if (current_state.is_final())
		{
			print_solution(current_state, states);
			return true;
		}
		if (current_depth > max_depth)
		{
			continue;
		}
		for (const auto& neighbor : current_state.get_neighbors())
		{
			State new_state = current_state.transition(neighbor);
			if (!visited_states.count(new_state.get_matrix()))
			{
				visited_states.insert(current_state.get_matrix());
				stack.push(std::make_pair(new_state, current_depth + 1));
			}
		}
	}
	return false;
}

void idfs(vector<vector<int>> matrix, int32_t depth)
{
	for (int i=0; i<depth ;i++)
	{
		if(dfs(matrix, i))
		{
			return;
		}
	}
}

void greedy(vector<vector<int>> matrix, double(*heuristic)(const vector<vector<int>>&, const vector<vector<int>>&))
{
	auto comparator = [](const auto& p1, const auto& p2) { return p1.first > p2.first; };
	std::priority_queue<std::pair<double, State>, std::vector<std::pair<double, State>>, decltype(comparator)> minHeap(comparator);
	State initial_state(matrix);
	vector<vector<int>> best_final = {};
	vector<double> distances{};
	const auto final_states = Helper::get_final_states(matrix.size(), matrix[0].size());
	int min = heuristic(matrix, final_states[0]);
	int index = 0;
	for (int i=1; i<final_states.size(); i++)
	{
		double ch = heuristic(matrix, final_states[i]);
		if ( ch< min)
		{
			min = ch;
			index = i;
		}
	}
	Helper::set_digits();
	Helper::final_state = final_states[index];
	set<vector<vector<int>>> visited_states;
	int steps = 0;
	minHeap.push({ 0, initial_state });
	while (!minHeap.empty())
	{
 		auto [discard, current_state] = minHeap.top();
		minHeap.pop();
		steps++;
		if(current_state.is_final())
		{
			print_solution(current_state, steps);
			return;
		}
		for(const auto& neighbor : current_state.get_neighbors())
		{
			State new_state = current_state.transition(neighbor);
			if (!visited_states.count(new_state.get_matrix()))
			{
				minHeap.push(std::make_pair(heuristic(new_state.get_matrix(),Helper::final_state), new_state));
				visited_states.insert(current_state.get_matrix());
			}
		}

	}

	
}



int main()
{
	std::cout << "DFS:\n";
	dfs({ {8,6,7},{2,5,4},{0,3,1} });
	dfs({ {2,7,5},{0,8,4},{3,1,6} });
	dfs({ {2,5,3},{1,0,6},{4,7,8} });
	std::cout << "-------------------\n";
	std::cout << "BFS:\n";
	bfs({ {8,6,7},{2,5,4},{0,3,1} });
	bfs({ {2,7,5},{0,8,4},{3,1,6} });
	bfs({ {2,5,3},{1,0,6},{4,7,8} });
	std::cout << "-------------------\n";
	std::cout << "IDFS:\n";
	idfs({ {8,6,7},{2,5,4},{0,3,1} }, 500);
	idfs({ {2,7,5},{0,8,4},{3,1,6} }, 500);
	idfs({ {2,5,3},{1,0,6},{4,7,8} }, 500);
	std::cout << "-------------------\n";
	std::cout << "Greedy-hamming_distance\n:";
	greedy({{8,6,7},{0,5,4},{2,3,1} }, hamming_distance);
	greedy({{2,7,5},{0,8,4},{3,1,6} }, hamming_distance);
	greedy({{2,5,3},{1,0,6},{4,7,8} }, hamming_distance);
	std::cout << "-------------------\n";
	std::cout << "Greedy-euclidean_distance\n:";
	greedy({ {8,6,7},{0,5,4},{2,3,1} }, euclidean_distance);
	greedy({ {2,7,5},{0,8,4},{3,1,6} }, euclidean_distance);
	greedy({ {2,5,3},{1,0,6},{4,7,8} }, euclidean_distance);
	std::cout << "-------------------\n";
	std::cout << "Greedy-manhattan_distance\n";
	greedy({ {8,6,7},{0,5,4},{2,3,1} }, manhattan_distance);
	greedy({ {2,7,5},{0,8,4},{3,1,6} }, manhattan_distance);
	greedy({ {2,5,3},{1,0,6},{4,7,8} }, manhattan_distance);
 
}
