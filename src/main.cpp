#include "app.hpp"

#include <iostream>

int main() {
    try {
        STC::Application app{};
        app.run();
    } catch(const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}