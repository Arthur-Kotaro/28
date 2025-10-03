#pragma once
#include <iostream>


#define SWIMMERS_NUMBER 6
#define DISTANCE 100

class Swimmer {
    std::string name;
    int number;
    float speed, spentTime;
public:
    Swimmer(int num);
    void swimTracker();
    float getTime() const;
    std::string getName() const;
    int getNumber() const;
};