# Loader 5095 - Multi-Display Fullscreen Loader

A high-performance, multi-display loader application with auto-update functionality and interactive configuration menu.

## Features

✨ **Multi-Display Support**
- Detect and use multiple monitors
- Create windows on each display simultaneously
- Fullscreen and windowed modes

🎮 **Interactive Menu System**
- Select target display
- Choose display mode (Single/Multi/Fullscreen/Windowed)
- Configure FPS limit (30, 60, 120, 144, 240)
- Customize animation style

🔄 **Auto-Update System**
- Check for updates from GitHub releases
- Download and verify updates
- Automatic installation (platform-dependent)

⚙️ **Configuration**
- JSON-based configuration file
- Persistent settings
- Animation customization
- Background color settings

## Requirements

- C++17 compatible compiler
- CMake 3.10+
- SDL2
- OpenGL
- libcurl
- nlohmann/json

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Running

### First Run (Interactive Menu)
```bash
./loader5095
```

The application will show an interactive menu where you can:
1. Select display
2. Select mode (single/multi/fullscreen)
3. Configure FPS
4. Check for updates
5. Start the loader

### Command-line Options
```bash
# Use configuration file
./loader5095 --config config.json

# Windowed mode
./loader5095 --windowed

# Set animation speed
./loader5095 --speed 1.5

# Check updates
./loader5095 --check-updates

# Set FPS limit
./loader5095 --fps 120
```

## Configuration

Edit `config.json` to customize:

```json
{
  "display": {
    "fullscreen": true,
    "target_display": 0,
    "multi_display_mode": [0, 1],
    "scale": 1.0
  },
  "graphics": {
    "fps_limit": 60,
    "vsync": true,
    "background_color": {
      "r": 0.1,
      "g": 0.1,
      "b": 0.1,
      "a": 1.0
    }
  },
  "animation": {
    "speed": 1.0,
    "style": "spinner"
  },
  "update": {
    "auto_update": true,
    "url": "https://api.github.com/repos/mostafac21y-blip/Loader-5095/releases/latest"
  }
}
```

## Controls

| Key | Action |
|-----|--------|
| ↑/↓ | Navigate menu |
| Enter | Select option |
| ESC | Exit / Back to main menu |

## File Structure

```
Loader-5095/
├── CMakeLists.txt              Build configuration
├── config.json                 Default configuration
├── README.md                   This file
├── include/
│   ├── loader.h               Main loader class
│   ├── display_manager.h       Display detection
│   ├── graphics.h              OpenGL rendering
│   ├── config_manager.h        Configuration handling
│   ├── update_manager.h        Auto-update system
│   └── menu.h                  Interactive menu
└── src/
    ├── main.cpp               Entry point
    ├── loader.cpp             Loader implementation
    ├── display_manager.cpp    Display manager
    ├── graphics.cpp           Graphics system
    ├── config_manager.cpp     Configuration
    ├── update_manager.cpp     Update system
    └── menu.cpp               Menu system
```

## Architecture

### Display Manager
Automatically detects connected displays and provides:
- Display count
- Display bounds and resolution
- Primary display identification
- Multi-monitor spanning

### Graphics System
OpenGL-based rendering with:
- Hardware acceleration
- VSync support
- Viewport management
- Frame rate control

### Update Manager
Automatic update checking with:
- GitHub API integration
- Download with progress tracking
- Checksum verification
- Secure installation

### Configuration Manager
JSON-based config system with:
- Auto-loading on startup
- Auto-saving on exit
- Validation and defaults
- Easy serialization

### Menu System
Interactive terminal-style menu with:
- Multiple selection screens
- Keyboard navigation
- Real-time settings
- Visual feedback

## Update System

The loader automatically checks for updates from:
```
https://api.github.com/repos/mostafac21y-blip/Loader-5095/releases/latest
```

Updates are verified using checksums before installation.

## Contributing

See CONTRIBUTING.md for guidelines.

## License

MIT License - See LICENSE file for details

## Author

mostafac21y-blip

## Version

1.0.0
