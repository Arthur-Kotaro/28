#pragma once

class Station;

class Train
{
    char trainID;
    unsigned int travelTime, startMotionDelay;
public:
    Train(int index);
    void trainMovement(Station *);
    char getTrainLetter();
};

class Station {
    std::mutex stationAccess;
    Train *trainOccupant;
public:
    Station();
    void StationOccupy(Train* InTrainOccupant);
};