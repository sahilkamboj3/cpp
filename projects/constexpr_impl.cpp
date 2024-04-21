/*
 * Declare possibility of evaluating value of the function or
 * variable at compile time. The variable is still const,
 * but we can evaluate at compile time.
 * 
 */

#include <iostream>
#include <chrono>

constexpr int add(int a, int b) { return a + b; }

int main()
{
    // can look this over in godbolt also
    constexpr int i = 7+9+16; // constexpr idea: do we want to do this expression at runtime (const)
                    // or perform it at compile time (constexpr)

    std::cout << i << std::endl;
    constexpr int j = add(i, i);
    std::cout << j << std::endl;
}