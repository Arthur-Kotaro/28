#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <vector>
#include "main1.hpp"

#define DEBUG

std::mutex IOControl;
std::mutex ResultControl;
std::vector<Swimmer*> result;

bool compare(float value, float reference, float epsilon) { return (value >= reference-epsilon)&&(value <= reference+epsilon); }

Swimmer::Swimmer(int num)
{
    number = num + 1;
    std::cout << "Enter name of swimmer number " << number << ": ";
    std::cin >> name;
    do {
        std::cout << "Enter speed of swimmer in m/s (range 1...2): ";
        std::cin >> speed;
    } while (speed < 1 || speed > 2);
}

void Swimmer::swimTracker()
{
    float distancePassed = 0;
    unsigned int secondsPassed = 0;
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        secondsPassed++;
        distancePassed = speed * (float)secondsPassed;
        if(distancePassed > DISTANCE || compare(distancePassed, DISTANCE, 0.01)) break;
        else {
            IOControl.lock();
            std::cout << "Sec " << secondsPassed << ": swimmer N " << number << ' ' << name << " has swam " << distancePassed << " meters." << std::endl;
            IOControl.unlock();
        }
    }
    spentTime = DISTANCE / speed;
    ResultControl.lock();
    result.push_back(this);
    ResultControl.unlock();
}

float Swimmer::getTime() const {return spentTime;}
std::string Swimmer::getName() const {return name;}
int Swimmer::getNumber() const {return number;}

int main()
{
    Swimmer* SWMarr[SWIMMERS_NUMBER];
    std::thread THRarr[SWIMMERS_NUMBER];
    for(int i = 0; i < SWIMMERS_NUMBER; i++)  SWMarr[i]= new Swimmer(i);
    for(int i = 0; i < SWIMMERS_NUMBER; i++) THRarr[i] = std::thread(&Swimmer::swimTracker, SWMarr[i]);
    for(auto & i : THRarr) i.join();
    for(auto & i : result) std::cout<< " Swimmer N" << i->getNumber() << ", " << i->getName() << "\'s result " << i->getTime() << " sec.\n";
    for(auto & i : SWMarr) delete i;
    return 0;
}