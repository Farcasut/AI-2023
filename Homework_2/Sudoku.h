//
// Created by farca on 10/28/2023.
//

#ifndef HOMEWORK_2_SUDOKU_H
#define HOMEWORK_2_SUDOKU_H
#include <vector>
#include <cstdint>
#include <unordered_set>

using std::unordered_set;
using std::vector;
class Sudoku
{
    vector<vector<int8_t>> board;
public:
     Sudoku(const std::vector<std::vector<int8_t>>& board)
    {
        this->board=board;
    }
    bool is_complete()
    {
        vector<unordered_set<uint8_t>> grid_set(9);
        //Check if the sudoku is complete;
        for (int i = 0; i < board.size(); i++) {
            unordered_set<uint8_t> row_set;
            unordered_set<uint8_t> col_set;
            for (int j = 0; j < board[0].size(); ++j) {
                if (board[i][j]==0)
                    return false;
                // check the row set
                if (row_set.find(board[i][j]) != row_set.end())
                    return false;
                row_set.insert(board[i][j]);
                // check the column set
                if (col_set.find(board[i][j])!=col_set.end())
                    return false;
                col_set.insert(board[i][j]);
                // To check the small grid we need to retrieve the right grid
                uint8_t sub_grid = i/3 + j/3;
                if(grid_set[sub_grid].find(board[i][j]) != grid_set[sub_grid].end())
                    return false;
                grid_set[sub_grid].insert(board[i][j]);
            }

        }
    }
};
#endif //HOMEWORK_2_SUDOKU_H
