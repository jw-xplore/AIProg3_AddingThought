#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;
const int fps = 5;
long long baseSleep = 0;
double prevDTime = 0;

void render()
{
    //system("CLS");
}

/*
Write all logic into this fuction
*/
void update(double dTime)
{
    //cout << "Game update: " << dTime << endl;
}

int main()
{
    baseSleep = (1.0f / (double)fps) * 1000;

    while (true)
    {
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