#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <windows.h>

#include "World.h"

using namespace std;
int fps = 1;
double timeChange = 0.1f;
long long baseSleep = 0;
double prevDTime = 0;

World* world;

void init()
{
    world = new World();
}

void render()
{
    cout << "Day: " << world->day << ", Time: " << world->time << " (Speed: " << fps << " FPS)" << endl;
    cout << "----------------------------------------" << endl;
    world->showPeopleStatus();
    cout << "----------------------------------------" << endl;
    cout << "Messages:" << endl;
    world->showMessagesLog();
}

/*
Write all logic into this fuction
*/
void update(double dTime)
{
    world->updateTime(timeChange);
}

int main()
{
    baseSleep = (1.0f / (double)fps) * 1000;
    init();

    while (true)
    {
        baseSleep = (1.0f / (double)fps) * 1000;

        // ESC - press check
        if (GetAsyncKeyState(27) & 0x8000)
            break;

        // Change update speed
        if (GetAsyncKeyState(VK_LEFT) & 0x8000 && fps > 1)
            fps--;

        if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            fps++;

        // Update game
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