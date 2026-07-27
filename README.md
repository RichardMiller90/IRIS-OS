# IRIS OS

A mobile operating system shell built with Qt6 Wayland Compositor for Linux.

## Architecture

```
iris-os/
├── compositor/          # Wayland compositor (system UI / window manager)
│   ├── main.cpp         # Entry point
│   ├── compositor.h/cpp # Core compositor logic
│   ├── appwindow.h/cpp  # Window management
│   ├── gesturehandler.h/cpp # Touch gesture recognition
│   └── virtualkeyboard.h/cpp # virtual keyboard
├── qml/                 # System UI QML files
│   ├── main.qml         # Root compositor UI
│   ├── Keyboard.qml     # Virtual keyboard container
│   ├── KeyButton.qml    # Individual key styling
│   ├── KeyPopup.qml     # Key press preview popup
│   ├── PredictiveBar.qml # Text prediction bar
│   ├── StatusBar.qml    # Top status bar
│   ├── HomeScreen.qml   # App launcher / wallpaper
│   ├── LockScreen.qml   # Lock screen
│   ├── ControlCenter.qml # System toggles panel
│   ├── NotificationPanel.qml
│   ├── AppWindowItem.qml
│   └── GestureHandler.qml
├── shared/              # App framework library
│   ├── mobileapplication.h/cpp
├── protocols/           # Wayland protocol definitions
├── CMakeLists.txt       # Build configuration
└── resources.qrc        # Qt resource file
```

## Prerequisites

- Qt6 (Core, Gui, Quick, WaylandCompositor, WaylandClient, OpenGL)
- CMake 3.21+
- C++20 compiler
- Linux with Wayland support
- Qt5Compat (for GraphicalEffects)

## Building

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Running

```bash
./mobile-compositor
```

Run Wayland clients with:
```bash
WAYLAND_DISPLAY=mobile-os qt-app
```

## Features

- **Wayland Compositor**: Full window management for mobile
- **Keyboard**: Translucent dark theme, rounded keys, predictive text, key popups
- **Gesture Navigation**: Swipe up for home, edge swipe for back
- **System UI**: Status bar, lock screen, control center, notifications
- **App Lifecycle**: Background suspension for battery saving
- **HiDPI Support**: Scale factor 3.0 for modern mobile displays

## Keyboard Shortcuts

- Swipe up from bottom: Home screen
- Swipe down from top: Control center
- Swipe from left edge: Back / close app
- Double-tap Shift: Caps lock
- Long press backspace: Rapid delete

---

*IRIS OS — See the future.*
