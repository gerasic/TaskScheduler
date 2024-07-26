#include <iostream>
#include <cmath>

#include "lib/TaskScheduler.hpp"


int main() {
    TaskScheduler scheduler;

    float a = 1;
    float b = -2;
    float c = 0;

    auto id1 = scheduler.Add([](float a, float c){return -4 * a * c;}, a, c);
    auto id2 = scheduler.Add([](float b, float v){return b * b + v;}, b, scheduler.GetFutureResult<float>(id1));
    auto id3 = scheduler.Add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));
    auto id4 = scheduler.Add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));
    auto id5 = scheduler.Add([](float a, float v){return v / (2 * a);}, a, scheduler.GetFutureResult<float>(id3));
    auto id6 = scheduler.Add([](float a, float v){return v / (2 * a);}, a, scheduler.GetFutureResult<float>(id4));

    std::cout << scheduler.GetResult<float>(id5) << "\n";   // 2
    std::cout << scheduler.GetResult<float>(id6) << "\n";   // 0

    return 0;
}
