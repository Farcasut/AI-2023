//
// Created by farca on 10/28/2023.
//

#ifndef HOMEWORK_2_DOMANIN_H
#define HOMEWORK_2_DOMANIN_H

#include <vector>

using std::vector;

class Domanin {
private:
    vector<int> available_values;
public:
    Domanin()
    {
        available_values={1,2,3,4,5,6,7,8,9};
    }

    void remove_value(int value)
    {
        for (auto it  = available_values.begin();  it != available_values.end();  it++ ) {
            available_values.erase(it);
            return;
        }
    }
};


#endif //HOMEWORK_2_DOMANIN_H
