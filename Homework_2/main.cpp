#include <iostream>
#include <vector>
#include <cstdint>
#include <unordered_set>
#include <queue>
#include <map>
#include "MostConstraint.h"

using std::priority_queue;
using std::unordered_set;
using std::vector;
using std::pair;


bool is_consistent(const vector<vector<int8_t>> &board, const int &i, const int &j) {
    // check row consistency
    for (int k = 0; k < 9; ++k) {
        if (board[i][k] == board[i][j] && k != j)
            return false;
    }
    // check column consistency
    for (int k = 0; k < 9; ++k) {
        if (board[k][j] == board[i][j] && k != i)
            return false;
    }

    int rStart = 3 * (i / 3);
    int cStart = 3 * (j / 3);
    for (int r = rStart; r < rStart + 3; r++) {
        for (int c = cStart; c < cStart + 3; c++) {
            if (board[r][c] == board[i][j] && (r != i || c != j))
                return false;
        }
    }

    return true;
}

//maybe I won't need to check everything
bool is_complete(const vector<vector<int8_t>> &board) {
    vector<unordered_set<uint8_t>> grid_set(9);
    vector<unordered_set<uint8_t>> col_set(9);

    //Check if the sudoku is complete;
    for (int i = 0; i < board.size(); i++) {
        unordered_set<uint8_t> row_set;
        for (int j = 0; j < board[i].size(); ++j) {

            uint8_t sub_grid = (i / 3) * 3 + j / 3;   
            //std::cout << "Sub matrix: " << (int)sub_grid << std::endl;
            if (grid_set[sub_grid].find(board[i][j]) != grid_set[sub_grid].end())
                return false;

            grid_set[sub_grid].insert(board[i][j]);

            if (board[i][j] == 0)
                return false;

            // check the row set
            if (row_set.find(board[i][j]) != row_set.end())
                return false;
            row_set.insert(board[i][j]);
            // check the column set
            if (col_set[j].find(board[i][j]) != col_set[j].end())
                return false;
            col_set[j].insert(board[i][j]);
            // To check the small grid we need to retrieve the right grid
        }
    }
    return true;
}

// maybe not a reference here
vector<vector<unordered_set<int>>> remove_from_domain(vector<vector<unordered_set<int>>> domains, const int &i, const int &j, const int &value) {
    //row
    for (int k = 0; k < 9; ++k) {
        domains[i][k].erase(value);
    }
    //col
    for (int k = 0; k < 9; ++k) {
        domains[k][j].erase(value);
    }
    //grid
    uint8_t rowStart = 3 * (i / 3);
    uint8_t rowEnd = rowStart + 3;
    uint8_t colStart = 3 * (j / 3);
    uint8_t colEnd = colStart + 3;
    for (uint8_t r = rowStart; r < rowEnd; r++) {
        for (uint8_t c = colStart; c < colEnd; c++) {
            domains[r][c].erase(value);
        }
    }
    return domains;
}


bool simple_bkt(vector<vector<int8_t>> board) {
    if (is_complete(board)) {
        return true;
    }
    for (int i = 0; i < 9; ++i) {

        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == 0) {
                for (int8_t val = 1; val <= 9; val++) {
                    if (is_consistent(board, i, j)) {
                        board[i][j] = val;
                        bool res = simple_bkt(board);
                        if (res)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

bool check_empty_domain(const vector<vector<unordered_set<int>>> &domains, const vector<vector<int8_t>> &board) {
    for (int i = 8; i >= 0; i--) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0 && domains[i][j].empty()) {
                return true;
            }
        }
    }
    return false;
}

std::pair<int, int> get_unassigned_location(const vector<vector<int8_t>>& board) {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}

bool forward_checking(vector<vector<int8_t>>& board, vector<vector<unordered_set<int>>> domains) {

    std::cout << "-----------[ BOARD ]----------\n";
    for (int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[i].size(); j++) {
            std::cout << (int)board[i][j] << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "------------------------------\n";
    
    auto location = get_unassigned_location(board);

    if (is_complete(board) || location == std::pair(-1, -1)) {
        for (int i = 0; i < board.size(); i++) {
            for(int j = 0; j < board[i].size(); j++) {
                std::cout << (int)board[i][j] << ' ';
            }
            std::cout << "\n";
        }
        return true;
    }
    
    auto const& [i, j] = location;

    for (const auto &val: domains[i][j]) {
        board[i][j] = val;
        // check for consistency
        if (is_consistent(board, i, j)) {
            auto new_domains = remove_from_domain(domains, i, j, board[i][j]);
            // if the domain of an unassigned cell is empty then we can return false;
            if (check_empty_domain(new_domains, board)) {
                board[i][j] = 0;
                continue;
            }
            bool res = forward_checking(board, new_domains);
            if (res)
                return true;
        }
        board[i][j] = 0;
    }
    return false;
}

bool MRV(vector<vector<int8_t>>& board, MostConstraint constrainer)
{

    std::cout << "-----------[ BOARD ]----------\n";
    for (int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[i].size(); j++) {
            std::cout << (int)board[i][j] << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "------------------------------\n";
    
    auto location = constrainer.get_next_location(board);

    if (is_complete(board) || location == std::pair(-1, -1)) {
        for (int i = 0; i < board.size(); i++) {
            for(int j = 0; j < board[i].size(); j++) {
                std::cout << (int)board[i][j] << ' ';
            }
            std::cout << "\n";
        }
        return true;
    }
    
    auto const& [i, j] = location;

    for (const auto &val: constrainer.lookup_domain_values(location)) {
        board[i][j] = val;
        // check for consistency
        if (is_consistent(board, i, j)) {

            auto new_constrainer = constrainer.update_domain(location, val);
            // if the domain of an unassigned cell is empty then we can return false;
            if (new_constrainer.check_empty_domains(board) == false) {
                board[i][j] = 0;
                continue;
            }
            bool res = MRV(board, new_constrainer);
            if (res)
                return true;
        }
        board[i][j] = 0;
    }
    return false;
}

int main() {
    vector<vector<int8_t>> correctSudoku = {
        {8, 4, 0, 0, 5, 0, 0, 0, 0},
        {3, 0, 0, 6, 0, 8, 0, 4, 0},
        {0, 0, 0, 4, 0, 9, 0, 0, 0},
        {0, 2, 3, 0, 0, 0, 9, 8, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 4},
        {0, 9, 8, 0, 0, 0, 1, 6, 0},
        {0, 0, 0, 5, 0, 3, 0, 0, 0},
        {0, 3, 0, 1, 0, 6, 0, 0, 7},
        {0, 0, 0, 0, 2, 0, 0, 1, 3}
    };

    vector<pair<int, int>> constraint_even_positions = { {2,2}, {0, 6}, {2,8}, {3, 4}, {4,3}, {4,5}, {5, 4}, {6,0}, {6, 6}, {8,2} };

    vector<vector<unordered_set<int>>> domains(9, vector<unordered_set<int>>(9, {1,2,3,4,5,6,7,8,9}));

    for (const auto& [line, column] : constraint_even_positions) {
        domains[line][column] = {2, 4, 6, 8};
    }

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (correctSudoku[i][j] != 0) {
                domains = remove_from_domain(domains, i, j, correctSudoku[i][j]);
            }
        }
    }

    MostConstraint x{};
    for (int i = 0; i < domains.size(); i++) {
        for(int j = 0; j < domains[i].size(); j++) {
            x.push({i, j}, domains[i][j]);
        }
    }
    x.sort(); 

    //    std::cout << simple_bkt(correctSudoku) << std::endl;
    std::cout << forward_checking(correctSudoku, domains) << std::endl;
    std::cout << MRV(correctSudoku, x) << std::endl;

    return 0;
}

