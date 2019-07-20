#include <iostream>

#include "renderer.h"

int main(int argc, char *argv[]) {
    constexpr std::size_t screenWidth{1280};
    constexpr std::size_t screenHeight{720};

    Renderer renderer(screenWidth, screenHeight);
    
    return 0;
}