#include "app.hpp"

#include <iostream>

int main() {
    try {
        STC::Application app{};
        app.run();
    } catch(std::exception& ex) {
        std::cout << ex.what();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}