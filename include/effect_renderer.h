#pragma once

#include <string>
#include <vector>

enum class EffectType {
    SPINNER,        // Rotating spinner
    PROGRESS_BAR,   // Progress bar animation
    PULSE,          // Pulsing effect
    WAVE,           // Wave animation
    PARTICLES,      // Particle system
    GRADIENT,       // Color gradient
    GLOW            // Glowing effect
};

class EffectRenderer {
public:
    EffectRenderer();
    ~EffectRenderer();
    
    // Initialize effect renderer
    bool initialize();
    
    // Render effect at given position and size
    void render_effect(EffectType effect, float x, float y, float width, float height, float time);
    
    // Render multiple effects
    void render_effects(const std::vector<EffectType>& effects, 
                       float x, float y, float width, float height, float time);
    
    // Set effect color
    void set_effect_color(float r, float g, float b, float a = 1.0f);
    
    // Enable/disable effects
    void set_effects_enabled(bool enabled);
    bool are_effects_enabled() const;
    
    // Set effect speed
    void set_effect_speed(float speed);
    float get_effect_speed() const;
    
    // Get available effects
    static const std::vector<std::string>& get_available_effects();
    
private:
    float color_r, color_g, color_b, color_a;
    bool effects_enabled;
    float effect_speed;
    
    // Render individual effects
    void render_spinner(float x, float y, float width, float height, float time);
    void render_progress_bar(float x, float y, float width, float height, float time);
    void render_pulse(float x, float y, float width, float height, float time);
    void render_wave(float x, float y, float width, float height, float time);
    void render_particles(float x, float y, float width, float height, float time);
    void render_gradient(float x, float y, float width, float height, float time);
    void render_glow(float x, float y, float width, float height, float time);
    
    // Utility functions
    void draw_circle(float x, float y, float radius, int segments = 32);
    void draw_rectangle(float x, float y, float width, float height);
    void draw_line(float x1, float y1, float x2, float y2);
};
