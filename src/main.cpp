#include "loader.h"
#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
    std::cout << "===== Loader 5095 =====" << std::endl;
    std::cout << "Multi-Display Fullscreen Loader" << std::endl;
    std::cout << "=====================" << std::endl;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create and configure loader
    Loader loader;

    // Configuration options
    bool fullscreen = true;
    float animation_speed = 1.0f;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--windowed") {
            fullscreen = false;
        } else if (arg == "--speed" && i + 1 < argc) {
            animation_speed = std::stof(argv[++i]);
        }
    }

    loader.set_fullscreen(fullscreen);
    loader.set_animation_speed(animation_speed);
    loader.set_background_color(0.1f, 0.1f, 0.1f, 1.0f);

    // Run loader
    loader.run();

    // Cleanup
    SDL_Quit();

    return 0;
}
