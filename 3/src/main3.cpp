#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <list>
#include <mutex>
#include <random>


enum Dish {pizza, soup, steak, salad, sushi};
std::list<Dish> orderedDishes;
std::list<Dish> readyDishes;

std::mutex OrderedDishListProtect;
std::mutex ReadyDishListProtect;
std::mutex IOControl;

std::string dishName(Dish dishID)
{
    if(dishID == pizza) return "pizza";
    else if(dishID == soup) return "soup";
    else if(dishID == steak) return "steak";
    else if(dishID == salad) return "salad";
    else if(dishID == sushi) return "sushi";
    else return "food scraps";
}


void getOrder(const unsigned int limit)
{
    for (int ordersNumber = 0; ordersNumber < limit; ordersNumber++)
    {
        Dish orderedDish;
        unsigned int orderDelay = std::rand() % 5 + 5;
        std::this_thread::sleep_for(std::chrono::seconds(orderDelay));
        orderedDish = (Dish) (std::rand() % 5);
        IOControl.lock();
        std::cout << "Received order N" << ordersNumber+1 << ": " << dishName(orderedDish) << std::endl;
        IOControl.unlock();
        OrderedDishListProtect.lock();
        orderedDishes.push_back(orderedDish);
        OrderedDishListProtect.unlock();
    }
}

void CookDish(const unsigned int limit)
{
    for (int ordersNumber = 0; ordersNumber < limit;)
    {
        if (!orderedDishes.empty())
        {
            unsigned int CookingDuration = std::rand() % 10 + 5;
            std::this_thread::sleep_for(std::chrono::seconds(CookingDuration));
            OrderedDishListProtect.lock();
            ReadyDishListProtect.lock();
            readyDishes.push_back(orderedDishes.front());
            IOControl.lock();
            std::cout << "Dish N" << ordersNumber+1 << " ready : " << dishName(orderedDishes.front()) << std::endl;
            IOControl.unlock();
            orderedDishes.pop_front();
            OrderedDishListProtect.unlock();
            ReadyDishListProtect.unlock();
            ordersNumber++;
        }
    }
}

void DeliveryDish(const unsigned int limit)
{
    for (int ordersNumber = 0; ordersNumber < limit;)
    {
        if(!readyDishes.empty())
        {
            ReadyDishListProtect.lock();
            IOControl.lock();
            std::cout << "Shipped order N" << ordersNumber+1 << ": " << dishName(readyDishes.front()) << std::endl;
            IOControl.unlock();
            readyDishes.pop_front();
            ReadyDishListProtect.unlock();
            ordersNumber++;
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    }
}

int main()
{
    std::cout << "Online restaurant FoodBox is open.\n";
    const unsigned int limit = 10;
    auto ordering = std::thread(getOrder, limit);
    auto cooking = std::thread(CookDish, limit);
    auto deliver = std::thread(DeliveryDish, limit);
    ordering.join();
    cooking.join();
    deliver.join();
    std::cout << "Online restaurant FoodBox is closed.\n";
    return 0;
}
