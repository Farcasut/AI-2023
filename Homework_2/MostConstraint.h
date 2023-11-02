#pragma once
#include <cstdint>
#include <vector>
#include <unordered_set>
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

using std::vector;
using std::pair;
using std::unordered_set;
using std::map;
using std::cout;
using std::make_pair;

class MostConstraint {
    vector<pair<pair<int, int>, unordered_set<int>*>> values_domains;
    map<pair<int, int>, unordered_set<int>> look_table;

public:

    MostConstraint copy(MostConstraint& c) {
        MostConstraint a;
        for(auto const& val : c.look_table) {
            a.push(val.first, val.second);
        }
        return a;
    }

    void push(pair<int, int> location, unordered_set<int> values) {
        this->look_table[location] = values;
        values_domains.emplace_back(make_pair(location, &this->look_table[location]));
    }

    pair<int, int> get_next_location(const vector<vector<int8_t>>& board) {
        for(auto const& val : values_domains) {
            if(board[val.first.first][val.first.second] == 0) {
                return {val.first.first, val.first.second};
            }
        }
        return {-1, -1};
    }

    unordered_set<int>& get_least_values_domain() {
        return *values_domains[0].second;
    } 

    pair<int, int> get_least_values_domain_coordonates() const {
        return values_domains[0].first;
    }
    
    void sort() {
        std::sort(this->values_domains.begin(), this->values_domains.end(), [](const auto& a, const auto& b) {
                    if (a.second->size() == 0) {
                        return false;
                    }
                    if (b.second->size() == 0) {
                        return true;
                    }
                    return a.second->size() < b.second->size();
        });
    }

    unordered_set<int>& lookup_domain_values(pair<int, int> location) {
        return this->look_table[location]; 
    } 

    MostConstraint update_domain(pair<int, int> location, int value) {
        MostConstraint new_object = this->copy(*this); 

        for(int j = 0; j < 9; j++) {
            new_object.look_table[{location.first, j}].erase(value);
        } 
        for(int i = 0; i < 9; i++) {
            new_object.look_table[{i, location.second}].erase(value);
        } 
        uint8_t rowStart = 3 * (location.first / 3);
        uint8_t rowEnd = rowStart + 3;
        uint8_t colStart = 3 * (location.second / 3);
        uint8_t colEnd = colStart + 3;

        for (uint8_t r = rowStart; r < rowEnd; r++) {
            for (uint8_t c = colStart; c < colEnd; c++) {
                new_object.look_table[{r,c}].erase(value);
            }
        }
        this->sort();
        return new_object;
    }

    bool check_empty_domains(const vector<vector<int8_t>>& board) {
        for (int i = 0; i < board.size(); i++) {
            for(int j = 0; j < board[i].size(); j++) {
                if (board[i][j] == 0 && this->look_table[{i,j}].empty()) 
                    return false;
            }
        } 
        return true;
    }

    void dump() const {
        cout << "-----------------[ Values Domains ] -----------------\n";
        for(int i = 0; i < values_domains.size(); i++) {
            cout << values_domains[i].first.first << ',' << values_domains[i].first.second << ": ";
            for(auto const& val : *values_domains[i].second) {
                cout << val << ' ';
            }
            cout << '\n';
        }
        cout << "-----------------[ END Domains ] -----------------\n";
        cout << "-----------------[ Lookup Table ] -----------------\n";
        for(auto const& val : this->look_table) {
            cout << val.first.first << ',' << val.first.second << ": ";
            for(auto const& x : val.second) {
                cout << x << ' ';
            }
            cout << '\n';
        }
        cout << "-----------------[ END Lookup Table ] -----------------\n";
    }
};
