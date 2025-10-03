#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <main2.hpp>
#include <string>

#define TRAINS_NUMBER 3
#define DEBUG

std::mutex IOControl;

Station::Station()
{
    trainOccupant = nullptr;
    stationAccess.unlock();
    std::cout << "Station is available.\n";
}

void Station::StationOccupy(Train* InTrainOccupant)
{
    stationAccess.lock();
    trainOccupant = InTrainOccupant;
    std::string input;
    IOControl.lock();
    std::cout << "Train " << trainOccupant->getTrainLetter() << " arrived on station.\n";
    do {
        std::cout << "If you are ready to depart the train " << trainOccupant->getTrainLetter() << " enter \"depart\" : ";
        std::cin >> input;
    }
    while(input != "depart");
    std::cout << "Train " << trainOccupant->getTrainLetter() << " departed.\n";
    IOControl.unlock();
    trainOccupant = nullptr;
    stationAccess.unlock();
}


Train::Train(int index)
{
    trainID = char('A' + index);
    std::cout << "Enter travel time of train " << trainID << ": ";
    std::cin >> travelTime;
    startMotionDelay = std::rand() % 6 + 1;
}

void Train::trainMovement(Station *InTrainstation)
{
    std::this_thread::sleep_for(std::chrono::seconds(startMotionDelay));
    IOControl.lock();
    std::cout<< "Train " << trainID << " pulled away.\n";
    IOControl.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(travelTime));
    IOControl.lock();
    std::cout<< "Train " << trainID << " is approaching to the station.\n";
    IOControl.unlock();
    InTrainstation->StationOccupy(this);
}

char Train::getTrainLetter() {return trainID;}

int main()
{
    Station Trainstation;
    Train* TrainArr[TRAINS_NUMBER];
    std::thread THRarr[TRAINS_NUMBER];
    for (int i = 0; i < TRAINS_NUMBER; ++i) { TrainArr[i] = new Train(i); }
    for (int i = 0; i < TRAINS_NUMBER; ++i) { THRarr[i] = std::thread(&Train::trainMovement, TrainArr[i], &Trainstation); }
#ifdef DEBUG
    std::cout << "DEBUG: Threads launched\n";
#endif
    for(auto & i : THRarr) i.join();
    for (auto & i : TrainArr) delete i;
    return 0;
}
