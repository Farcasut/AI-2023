#include <iostream>
#include <vector>
#include <cstdint>
#include <unordered_set>
#include "iostream"
#include <queue>
#include <map>

using std::priority_queue;
using std::unordered_set;
using std::vector;
using std::pair;


bool is_consistent(const vector<vector<int8_t>> &board, const int &i, const int &j, const int &val) {
    // check row consistency
    for (int k = 0; k < 9; ++k) {
        if (board[i][k] == val)
            return false;
    }
    // check column consistency
    for (int k = 0; k < 9; ++k) {
        if (board[k][j] == val)
            return false;
    }
    // check sub-grid consistency
    uint8_t r = 3 * (i / 3);
    uint8_t rl = r + 3;
    uint8_t c = 3 * (j / 3);
    uint8_t cl = c + 3;
    for (; r < rl; r++) {
        for (; c < cl; c++) {
            if (board[r][c] == val)
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
        for (int j = 0; j < board[0].size(); ++j) {
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
            uint8_t sub_grid = 3 * (i / 3) + j / 3;
            if (grid_set[sub_grid].find(board[i][j]) != grid_set[sub_grid].end())
                return false;
            grid_set[sub_grid].insert(board[i][j]);
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
                    if (is_consistent(board, i, j, val)) {
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

bool check_empty_domain(const vector<vector<unordered_set<int>>> &domains, const vector<vector<int8_t>> &board, int i, int j) {
    for (; i < 9; i++) {
        for (; j < 9; j++) {
            if (board[i][j] == 0 && domains[i][j].empty()) {
                return true;
            }
        }
    }
    return false;
}

bool forward_checking(vector<vector<int8_t>> board, vector<vector<unordered_set<int>>> domains) {
    if (is_complete(board))
        return true;
    for (int i = 0; i < 9; ++i) {

        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == 0) {
                for (const auto &val: domains[i][j]) {
                    // check for consistency
                    if (is_consistent(board, i, j, val)) {
                        board[i][j] = val;
                        auto new_domains = remove_from_domain(domains, i, j, board[i][j]);
                        // if the domain of an unassigned cell is empty then we can return false;
                        if (check_empty_domain(new_domains, board, i, j)) {
                            return false;
                        }
                        bool res = forward_checking(board, new_domains);
                        if (res)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

bool MRV(vector<vector<int8_t>> board, vector<vector<unordered_set<int>>> domains)
{
    // to get the variable we could loop through all the items that are 0 and check the one with the smallest domain?
}
int main() {
    vector<vector<int8_t>> correctSudoku = {
            {5, 3, 4, 6, 7, 8, 9, 1, 2},
            {6, 7, 2, 1, 9, 5, 3, 4, 8},
            {1, 9, 8, 0, 4, 2, 0, 6, 7},
            {8, 5, 9, 7, 6, 1, 4, 2, 3},
            {4, 2, 6, 0, 5, 3, 7, 9, 1},
            {7, 0, 3, 9, 2, 4, 0, 5, 6},
            {9, 6, 1, 5, 3, 7, 2, 8, 4},
            {2, 8, 7, 0, 1, 9, 6, 3, 5},
            {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };
    vector<vector<unordered_set<int>>> domains(9, vector<unordered_set<int>>(9, {1,2,3,4,5,6,7,8,9}));
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (correctSudoku[i][j] != 0) {
                domains = remove_from_domain(domains, i, j, correctSudoku[i][j]);
            }
        }
    }
    auto comparator = [](const auto& p1, const auto&p2){return p1.second.size()>p2.second.size();};
    std::cout << forward_checking(correctSudoku, domains) << std::endl;
    return 0;
}

