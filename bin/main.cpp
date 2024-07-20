#include <iostream>
#include "lib/MyStdAny.hpp"

int main() {
    Any a = 10;
    Any b = "FSDF";
    Any c = 1.0;

    std::cout << a<int>() << "\n";
}