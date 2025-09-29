#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#define SWIMMERS_NUMBER 6
#define DISTANCE 100


struct Swimmer { std::string name; float speed; float result; };

bool compare(float value, float reference, float epsilon) { return (value >= reference-epsilon)&&(value <= reference+epsilon); }

void swimTracker(const int num, const std::string &name, const float speed)
{
    float distancePassed = 0;
    unsigned int secondsPassed = 0;
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        distancePassed = speed * ++secondsPassed;
        if(!compare(distancePassed, DISTANCE, 0.01))
            std::cout << "Sec " << secondsPassed << ": swimmer N " << num << ' ' << name << " has swam " << distancePassed << " meters." << std::endl;
        else break;
    }
    return;
}


int main()
{
    Swimmer SWMarr[SWIMMERS_NUMBER];
    for(int i = 0; i < SWIMMERS_NUMBER; i++) {
     std::cout << "Enter name of swimmer number " << i+1 << ": ";
     std::getline(std::cin, SWMarr[i].name);
     do{
        std::cout << "Enter speed of swimmer in m/s (range 1...2): ";
        std::cin >> SWMarr[i].speed;
     } while(SWMarr[i].speed < 1 || SWMarr[i].speed > 2);
    }
    std::thread THRarr[SWIMMERS_NUMBER];
    for(int i = 0;i < SWIMMERS_NUMBER; i++) THRarr[i] = std::thread(swimTracker, i+1, SWMarr[i].name, SWMarr[i].speed);
    for(int i = 0;i < SWIMMERS_NUMBER; i++) THRarr[i].join();
    return 0;
}
