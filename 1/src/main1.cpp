#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#define SWIMMERS_NUMBER 6
#define DISTANCE 100

//#define DEBUG

bool compare(float value, float reference, float epsilon) { return (value >= reference-epsilon)&&(value <= reference+epsilon); }

class Swimmer {
    std::string name;
    int number;
    float speed, result;
public:
    Swimmer(int num)
    {
        number = num + 1;
        std::cout << "Enter name of swimmer number " << number << ": ";
//        std::getline(std::cin, name);
//        std::cin.clear();
        std::cin >> name;
        do {
            std::cout << "Enter speed of swimmer in m/s (range 1...2): ";
            std::cin >> speed;
        } while (speed < 1 || speed > 2);
#ifdef DEBUG
        std::cout<< "DEBUG: constuctor: Swimmer N" << number << ", name " << name << ", speed: " << speed  << ".\n";
#endif
    }
    void swimTracker()
    {
        float distancePassed = 0;
        unsigned int secondsPassed = 0;
#ifdef DEBUG
        std::cout<< "DEBUG: swimmer N" << number << " thread started\n";
#endif
        while(true)
        {
#ifdef DEBUG
            std::cout<< "DEBUG: swimmer N" << number << " thread: second: " << secondsPassed << "\n";
#endif
            std::this_thread::sleep_for(std::chrono::seconds(1));
            secondsPassed++;
            distancePassed = speed * (float)secondsPassed;
            //distancePassed = speed * ++secondsPassed;
            if(distancePassed > DISTANCE || compare(distancePassed, DISTANCE, 0.01)) break;
            else
                std::cout << "Sec " << secondsPassed << ": swimmer N " << number << ' ' << name << " has swam " << distancePassed << " meters." << std::endl;
        }
        result = DISTANCE / speed;
    }
    float getResult() const {return result;}
    std::string getName() {return name;}
    int getNumber() const {return number;}
};

void sortSWMarrPtr(Swimmer** arr, unsigned int arrSize)
{
    if (arrSize >= 2)
    {
        for (int i = 1; i < arrSize; ++i)
        {
            if (arr[0]->getResult() > arr[i]->getResult())
            {
                Swimmer* tmp = arr[0];
                arr[0] = arr[i];
                arr[i] = tmp;
            }
        }
        sortSWMarrPtr(++arr, --arrSize);
    }
}


int main()
{
    Swimmer* SWMarr[SWIMMERS_NUMBER];
    std::thread THRarr[SWIMMERS_NUMBER];
    for(int i = 0; i < SWIMMERS_NUMBER; i++) SWMarr[i] = new Swimmer(i);
#ifdef DEBUG
    std::cout<< "DEBUG: constuctors OK\n";
    for(int i = 0; i < SWIMMERS_NUMBER; i++) std::cout<< " Swimmer N" << SWMarr[i]->getNumber() << ", " << SWMarr[i]->getName() << ".\n";
    //for(auto & i : SWMarr) std::cout<< " Swimmer N" << i->getNumber() << ", " << i->getName() << ".\n";
#endif
    for(int i = 0; i < SWIMMERS_NUMBER; i++) THRarr[i] = std::thread(&Swimmer::swimTracker, SWMarr[i]);
#ifdef DEBUG
    std::cout<< "DEBUG: Threads created\n";
#endif
    for(int i = 0; i < SWIMMERS_NUMBER; i++) THRarr[i].join();
#ifdef DEBUG
    std::cout<< "DEBUG: Threads jointed\n";
#endif
    sortSWMarrPtr(SWMarr, SWIMMERS_NUMBER);
#ifdef DEBUG
    std::cout<< "DEBUG: SWMarr sorted\n";
#endif
    for(auto & i : SWMarr) std::cout<< " Swimmer N" << i->getNumber() << ", " << i->getName() << "\'s result " << i->getResult() << " sec.\n";
    for(auto & i : SWMarr) delete i;
    return 0;
}
