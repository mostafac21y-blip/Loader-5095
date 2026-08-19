#include "graphics.h"
#include <iostream>
#include <GL/glu.h>

Graphics::Graphics() : gl_context(nullptr), initialized(false) {}

Graphics::~Graphics() {
    shutdown();
}

bool Graphics::initialize(int width, int height, bool fullscreen) {
    if (initialized) {
        return true;
    }

    std::cout << "Initializing graphics context (" << width << "x" << height << ")"
              << (fullscreen ? " - Fullscreen" : "") << std::endl;

    initialized = true;
    return true;
}

SDL_GLContext Graphics::create_context(SDL_Window* window) {
    if (!window) {
        std::cerr << "Error: Invalid window pointer" << std::endl;
        return nullptr;
    }

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_GL_SetSwapInterval(1); // Enable vsync

    if (!setup_opengl()) {
        SDL_GL_DeleteContext(gl_context);
        gl_context = nullptr;
        return nullptr;
    }

    std::cout << "OpenGL Context created successfully" << std::endl;
    std::cout << "Version: " << get_gl_version() << std::endl;

    return gl_context;
}

void Graphics::set_viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Graphics::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::present(SDL_Window* window) {
    if (window && gl_context) {
        SDL_GL_SwapWindow(window);
    }
}

void Graphics::shutdown() {
    if (gl_context) {
        SDL_GL_DeleteContext(gl_context);
        gl_context = nullptr;
    }
    initialized = false;
}

const char* Graphics::get_gl_version() const {
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

bool Graphics::setup_opengl() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_SMOOTH);
    glShadeModel(GL_SMOOTH);

    return true;
}
