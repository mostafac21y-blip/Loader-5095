#include "loader.h"
#include <iostream>
#include <cmath>
#include <chrono>

Loader::Loader()
    : is_running(false), fullscreen_mode(false), animation_speed(1.0f) {
    background_color[0] = 0.1f;
    background_color[1] = 0.1f;
    background_color[2] = 0.1f;
    background_color[3] = 1.0f;

    display_manager = std::make_unique<DisplayManager>();
}

Loader::~Loader() {
    cleanup();
}

bool Loader::initialize() {
    std::cout << "Initializing Loader 5095..." << std::endl;

    if (!display_manager->initialize()) {
        std::cerr << "Failed to initialize display manager" << std::endl;
        return false;
    }

    // If no target displays specified, use all displays
    if (target_displays.empty()) {
        for (int i = 0; i < display_manager->get_display_count(); ++i) {
            target_displays.push_back(i);
        }
    }

    if (!create_windows()) {
        std::cerr << "Failed to create windows" << std::endl;
        return false;
    }

    return true;
}

bool Loader::create_windows() {
    for (int display_idx : target_displays) {
        const Display& display = display_manager->get_display(display_idx);

        int window_flags = SDL_WINDOW_OPENGL;
        if (fullscreen_mode) {
            window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }

        SDL_Window* window = SDL_CreateWindow(
            "Loader 5095",
            display.bounds.x,
            display.bounds.y,
            display.bounds.w,
            display.bounds.h,
            window_flags);

        if (!window) {
            std::cerr << "Failed to create window for display " << display_idx
                      << ": " << SDL_GetError() << std::endl;
            return false;
        }

        windows.push_back(window);

        auto graphics = std::make_unique<Graphics>();
        if (!graphics->initialize(display.bounds.w, display.bounds.h, fullscreen_mode)) {
            std::cerr << "Failed to initialize graphics for display " << display_idx << std::endl;
            SDL_DestroyWindow(window);
            return false;
        }

        SDL_GLContext context = graphics->create_context(window);
        if (!context) {
            std::cerr << "Failed to create GL context for display " << display_idx << std::endl;
            SDL_DestroyWindow(window);
            return false;
        }

        graphics->set_viewport(0, 0, display.bounds.w, display.bounds.h);
        graphics_contexts.push_back(std::move(graphics));

        std::cout << "Created window for display " << display_idx << std::endl;
    }

    return !windows.empty();
}

void Loader::set_fullscreen(bool fullscreen) {
    fullscreen_mode = fullscreen;
}

void Loader::set_target_displays(const std::vector<int>& displays) {
    target_displays = displays;
}

void Loader::set_animation_speed(float speed) {
    animation_speed = speed;
}

void Loader::set_background_color(float r, float g, float b, float a) {
    background_color[0] = r;
    background_color[1] = g;
    background_color[2] = b;
    background_color[3] = a;
}

void Loader::run() {
    if (!initialize()) {
        std::cerr << "Failed to initialize loader" << std::endl;
        return;
    }

    is_running = true;
    auto last_time = std::chrono::high_resolution_clock::now();

    std::cout << "Starting loader animation..." << std::endl;

    while (is_running) {
        auto current_time = std::chrono::high_resolution_clock::now();
        float delta_time =
            std::chrono::duration<float>(current_time - last_time).count();
        last_time = current_time;

        handle_events();
        update(delta_time);
        render();

        // Cap frame rate at 60 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    cleanup();
    std::cout << "Loader stopped" << std::endl;
}

void Loader::stop() {
    is_running = false;
}

void Loader::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                }
                break;
            default:
                break;
        }
    }
}

void Loader::update(float delta_time) {
    // Animation logic can be added here
    // For example: rotate spinner, pulse effects, etc.
}

void Loader::render() {
    for (size_t i = 0; i < windows.size(); ++i) {
        if (i < graphics_contexts.size()) {
            auto& graphics = graphics_contexts[i];

            // Make context current
            SDL_GL_MakeCurrent(windows[i], graphics->gl_context);

            // Clear screen
            graphics->clear(background_color[0], background_color[1],
                           background_color[2], background_color[3]);

            // Draw loader animation
            // For now, just clearing the screen
            // TODO: Add actual loader graphics (spinner, progress bar, etc.)

            // Present frame
            graphics->present(windows[i]);
        }
    }
}

void Loader::cleanup() {
    for (auto window : windows) {
        SDL_DestroyWindow(window);
    }
    windows.clear();
    graphics_contexts.clear();
}
