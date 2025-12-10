#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <windows.h>

#include "World.h"

using namespace std;
const int fps = 5;
long long baseSleep = 0;
double prevDTime = 0;

World* world;

void init()
{
    world = new World();
}

void render()
{
    cout << "Day: " << world->day << ", Time: " << world->time << endl;
    cout << "----------------------------------------" << endl;
    world->showPeopleStatus();
}

/*
Write all logic into this fuction
*/
void update(double dTime)
{
    world->updateTime(dTime, 0.25f);
}

int main()
{
    baseSleep = (1.0f / (double)fps) * 1000;
    init();

    while (true)
    {
        // ESC - press check
        if (GetAsyncKeyState(27) & 0x8000)
            break;

        system("CLS");
        auto start = chrono::system_clock::now();
        update(prevDTime);
        render();
        auto end = chrono::system_clock::now();

        double dif = chrono::duration<double>(end - start).count() * 1000;
        long long sleepTime = baseSleep - dif;
        if (sleepTime < 0)
            sleepTime = 0;

        prevDTime = (double)sleepTime / (double)1000;
        this_thread::sleep_for(chrono::milliseconds(sleepTime));
    }
}