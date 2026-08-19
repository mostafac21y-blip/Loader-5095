#include "effect_renderer.h"
#include <iostream>
#include <cmath>
#include <GL/gl.h>

EffectRenderer::EffectRenderer()
    : color_r(1.0f), color_g(1.0f), color_b(1.0f), color_a(1.0f),
      effects_enabled(true), effect_speed(1.0f) {}

EffectRenderer::~EffectRenderer() {}

bool EffectRenderer::initialize() {
    std::cout << "Effect renderer initialized" << std::endl;
    return true;
}

void EffectRenderer::render_effect(EffectType effect, float x, float y, float width,
                                   float height, float time) {
    if (!effects_enabled) return;

    glPushMatrix();
    glTranslatef(x, y, 0);

    switch (effect) {
        case EffectType::SPINNER:
            render_spinner(0, 0, width, height, time);
            break;
        case EffectType::PROGRESS_BAR:
            render_progress_bar(0, 0, width, height, time);
            break;
        case EffectType::PULSE:
            render_pulse(0, 0, width, height, time);
            break;
        case EffectType::WAVE:
            render_wave(0, 0, width, height, time);
            break;
        case EffectType::PARTICLES:
            render_particles(0, 0, width, height, time);
            break;
        case EffectType::GRADIENT:
            render_gradient(0, 0, width, height, time);
            break;
        case EffectType::GLOW:
            render_glow(0, 0, width, height, time);
            break;
    }

    glPopMatrix();
}

void EffectRenderer::render_effects(const std::vector<EffectType>& effects, float x,
                                    float y, float width, float height, float time) {
    for (const auto& effect : effects) {
        render_effect(effect, x, y, width, height, time);
    }
}

void EffectRenderer::set_effect_color(float r, float g, float b, float a) {
    color_r = r;
    color_g = g;
    color_b = b;
    color_a = a;
}

void EffectRenderer::set_effects_enabled(bool enabled) {
    effects_enabled = enabled;
    std::cout << "Effects " << (enabled ? "enabled" : "disabled") << std::endl;
}

bool EffectRenderer::are_effects_enabled() const {
    return effects_enabled;
}

void EffectRenderer::set_effect_speed(float speed) {
    effect_speed = speed;
}

float EffectRenderer::get_effect_speed() const {
    return effect_speed;
}

const std::vector<std::string>& EffectRenderer::get_available_effects() {
    static const std::vector<std::string> effects = {
        "Spinner",
        "Progress Bar",
        "Pulse",
        "Wave",
        "Particles",
        "Gradient",
        "Glow"
    };
    return effects;
}

void EffectRenderer::render_spinner(float x, float y, float width, float height,
                                    float time) {
    float rotation = time * 180.0f * effect_speed;
    glPushMatrix();
    glTranslatef(width / 2, height / 2, 0);
    glRotatef(rotation, 0, 0, 1);

    glColor4f(color_r, color_g, color_b, color_a);

    // Draw rotating circle
    float radius = std::min(width, height) / 3;
    draw_circle(0, 0, radius, 32);

    // Draw rotating arc
    glColor4f(color_r, color_g, color_b, color_a * 0.5f);
    draw_circle(0, 0, radius * 0.7f, 32);

    glPopMatrix();
}

void EffectRenderer::render_progress_bar(float x, float y, float width, float height,
                                         float time) {
    float progress = fmod(time * effect_speed, 1.0f);

    // Draw background
    glColor4f(color_r, color_g, color_b, color_a * 0.2f);
    draw_rectangle(0, height / 2 - 5, width, 10);

    // Draw progress
    glColor4f(color_r, color_g, color_b, color_a);
    draw_rectangle(0, height / 2 - 5, width * progress, 10);
}

void EffectRenderer::render_pulse(float x, float y, float width, float height,
                                  float time) {
    float scale = 0.8f + 0.2f * sin(time * 3.14159f * 2.0f * effect_speed);
    float radius = std::min(width, height) / 2 * scale;

    glColor4f(color_r, color_g, color_b, color_a * (1.0f - scale / 1.0f));
    draw_circle(width / 2, height / 2, radius, 32);
}

void EffectRenderer::render_wave(float x, float y, float width, float height,
                                 float time) {
    glColor4f(color_r, color_g, color_b, color_a);

    float amplitude = height / 4;
    float frequency = 2.0f * 3.14159f / width;

    glBegin(GL_LINE_STRIP);
    for (float px = 0; px < width; px += 2) {
        float py = height / 2 + sin((px + time * 50.0f * effect_speed) * frequency) * amplitude;
        glVertex2f(px, py);
    }
    glEnd();
}

void EffectRenderer::render_particles(float x, float y, float width, float height,
                                      float time) {
    glColor4f(color_r, color_g, color_b, color_a);

    for (int i = 0; i < 20; i++) {
        float angle = (time * 180.0f * effect_speed + i * 18.0f) * 3.14159f / 180.0f;
        float px = width / 2 + cos(angle) * width / 3;
        float py = height / 2 + sin(angle) * height / 3;
        draw_circle(px, py, 2, 8);
    }
}

void EffectRenderer::render_gradient(float x, float y, float width, float height,
                                     float time) {
    glBegin(GL_QUADS);
    float hue = fmod(time * effect_speed, 1.0f);

    glColor4f(color_r, color_g, color_b, color_a);
    glVertex2f(0, 0);
    glVertex2f(width, 0);
    glColor4f(color_r * 0.5f, color_g * 0.5f, color_b * 0.5f, color_a * 0.5f);
    glVertex2f(width, height);
    glVertex2f(0, height);
    glEnd();
}

void EffectRenderer::render_glow(float x, float y, float width, float height,
                                 float time) {
    float intensity = 0.5f + 0.5f * sin(time * 3.14159f * 2.0f * effect_speed);
    float radius = std::min(width, height) / 2;

    // Draw multiple circles with decreasing intensity
    for (int i = 0; i < 3; i++) {
        float alpha = color_a * intensity / (i + 1);
        glColor4f(color_r, color_g, color_b, alpha);
        draw_circle(width / 2, height / 2, radius - i * 5, 32);
    }
}

void EffectRenderer::draw_circle(float x, float y, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

void EffectRenderer::draw_rectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void EffectRenderer::draw_line(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}
