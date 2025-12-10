// Sample C++ Source Code for Compiler Simulation
#include <iostream>

int main() {
    int x = 10;
    int y = 20;
    int sum = x + y;
    
    std::cout << "Sum: " << sum << std::endl;
    
    if (sum > 25) {
        std::cout << "Sum is greater than 25" << std::endl;
    } else {
        std::cout << "Sum is less than or equal to 25" << std::endl;
    }
    
    for (int i = 0; i < 5; i++) {
        std::cout << "Iteration " << i << std::endl;
    }
    
    return 0;
}
