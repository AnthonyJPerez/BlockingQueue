
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

#include "../inc/Logger.h"
#include "../inc/BlockingQueue.hpp"


BlockingQueue<std::string> queue_{};
bool done = false;


static void push(std::string str)
{
    while (!done)
    {
        queue_.push(str);
    }
    LOG("push thread exiting...");
}


static void pop()
{
    while (!done)
    {
        queue_.pop();
    }
    LOG("pop thread exiting...");
}


int main()
{
    LOG("Starting...");

    std::vector<std::thread> threads;

    threads.push_back(std::thread(pop));
    threads.push_back(std::thread(pop));
    threads.push_back(std::thread(pop));
    threads.push_back(std::thread(push, "A"));
    threads.push_back(std::thread(push, "B"));
    threads.push_back(std::thread(push, "C"));
    threads.push_back(std::thread(push, "D"));
    threads.push_back(std::thread(push, "E"));
    threads.push_back(std::thread(push, "F"));

    LOG("Press enter to quit...");
    std::getchar();

    LOG("Quit signal received! Joining threads...");
    done = true;
    queue_.setBlocking(false);

    std::for_each(threads.begin(), threads.end(), [](std::thread &t) { t.join(); });
    LOG("Quitting...");
    return 0;
}