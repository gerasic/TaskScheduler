#include <gtest/gtest.h>
#include "TaskScheduler.hpp"
#include <cmath>


TEST(TaskSchedulerTest, AddTaskWithNoArguments) {
    TaskScheduler scheduler;

    auto index = scheduler.Add([]() { return 42; });

    ASSERT_EQ(scheduler.GetResult<int>(index), 42);
}


TEST(TaskSchedulerTest, TaskWithOneArgument) {
    TaskScheduler scheduler;
    auto id1 = scheduler.Add([](int x) { return x * 2; }, 5);

    auto x1 = scheduler.GetResult<int>(id1);

    ASSERT_EQ(x1, 10);
}


TEST(TaskSchedulerTest, TaskWithTwoArguments) {
    TaskScheduler scheduler;
    auto id1 = scheduler.Add([](int x, int y) { return x + y; }, 3, 5);

    auto x1 = scheduler.GetResult<int>(id1);
    ASSERT_EQ(x1, 8);
}


TEST(TaskSchedulerTest, MixedArguments) {
    TaskScheduler scheduler;

    auto id1 = scheduler.Add([](int i){return i * i;}, 10);
    auto id2 = scheduler.Add([](int i, int j){return i * j;}, scheduler.GetFutureResult<int>(id1), 3);

    auto x1 = scheduler.GetResult<int>(id2);
    ASSERT_EQ(x1, 300);
}


TEST(TaskSchedulerTest, TestFromExample) {
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

    auto x1 = scheduler.GetResult<float>(id5);
    auto x2 = scheduler.GetResult<float>(id6);

    ASSERT_EQ(x1, 2);
    ASSERT_EQ(x2, 0);
}


TEST(TaskSchedulerTest, FutureResultTest) {
    TaskScheduler scheduler;

    auto id1 = scheduler.Add([](float a, float c){return -4 * a * c;}, 1.0f, -2.0f);

    decltype(id1) id2 = id1;
    decltype(id1) id3 = id1;
    decltype(id1) id4 = id1;

    id2 = scheduler.Add([](float a, float c){return -4 * a * c;}, 1.0f, scheduler.GetFutureResult<float>(id3));     // 128
    id3 = scheduler.Add([](float a, float c){return -4 * a * c;}, 1.0f, scheduler.GetFutureResult<float>(id4));     // -32    
    id4 = scheduler.Add([](float a, float c){return -4 * a * c;}, 1.0f, -2.0f);                                     // 8

    auto x1 = scheduler.GetResult<float>(id2);

    ASSERT_EQ(x1, 128);
}


TEST(TaskSchedulerTest, MoveSemanticsTest) {
    TaskScheduler scheduler;
    std::string str1 = "string";
    auto id1 = scheduler.Add(
        [](const std::string& a, const std::string& b){return a + b;},
        static_cast<std::string&&>(str1),
        "string");

    auto x1 = scheduler.GetResult<std::string>(id1);


    ASSERT_EQ(x1, "stringstring");
    ASSERT_TRUE(str1.empty());
} 


#include "TestType.hpp"

TEST(TaskSchedulerTest, CustomTypeTest) {
    MyType<std::string> a{"Hello,"};
    MyType<std::string> b{" "};
    MyType<std::string> c{"World!"};

    TaskScheduler scheduler;
    
    auto id1 = scheduler.Add([](const MyType<std::string>& x, const MyType<std::string>& y){return x + y;}, a, b);

    auto id2 = scheduler.Add([](
        const MyType<std::string>& x, const MyType<std::string>& y){return x + y;}, 
        scheduler.GetFutureResult<MyType<std::string>>(id1), 
        c);

    auto x1 = scheduler.GetResult<MyType<std::string>>(id2);

    ASSERT_EQ(x1.GetValue(), "Hello, World!");
} 
