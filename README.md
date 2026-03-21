# Vertex

A minimal terminal-based side-scroller in C++20 using ncurses. Built as a learning project for **clean architecture**, **design patterns**, and **modern C++** practices.

## What This Is

Vertex is a simple ASCII-art platformer that runs in the terminal. You control a stick-figure character running and jumping across the screen. The game itself is minimal—the real focus is on the **software engineering**:

- **Abstraction**: Physics and rendering are fully decoupled (MVC); game logic has no knowledge of ncurses
- **Design patterns**: Command Pattern for input, State Pattern for player movement
- **Modern C++**: C++20, smart pointers only, `std::chrono` for frame timing
- **Game-dev basics**: Delta-time game loop, back-buffered rendering, sprite handling

## Architecture Overview

```
Input → Command → Player (Model) → PhysicsEngine updates positions → Renderer draws
```

| Layer       | Components                              | Responsibility                     |
|------------|------------------------------------------|-------------------------------------|
| **Model**  | GameWorld, Player, PhysicsEngine         | Game state, physics, no I/O        |
| **View**   | Renderer, BackBuffer, Sprite             | Read positions, draw to terminal    |
| **Controller** | InputHandler, Commands              | Map keys to actions                 |

## Controls

| Key | Action |
|-----|--------|
| Arrow keys / A / D | Move left / right |
| Space / Up | Jump |
| Q / Esc | Quit |

---

## Install & Run

### Linux

**1. Install dependencies**

```bash
# Debian / Ubuntu
sudo apt install build-essential cmake libncurses-dev

# Fedora
sudo dnf install gcc-c++ cmake ncurses-devel

# Arch
sudo pacman -S base-devel cmake ncurses
```

**2. Build**

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

**3. Run**

```bash
./bin/vertex
```

### Windows

**Option A: vcpkg (recommended)**

1. [Install vcpkg](https://vcpkg.io/en/getting-started.html)
2. Install PDCurses (ncurses-compatible library for Windows):

   ```powershell
   vcpkg install pdcurses:x64-windows
   ```

3. Build with CMake, pointing to the vcpkg toolchain:

   ```powershell
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build . --config Release
   ```

   Run with `.\bin\Release\vertex.exe`

**Option B: WSL (Windows Subsystem for Linux)**

Use the Linux instructions above inside a WSL terminal. The game runs identically.

---

## Requirements

- CMake 3.16+
- C++20 compiler (GCC 10+, Clang 10+, MSVC 2019+)
- ncurses (Linux/macOS) or PDCurses (Windows)

---

## Project Layout

```
vertex/
├── src/
│   ├── main.cpp          # Entry point
│   ├── Engine.*          # ncurses init, subsystems
│   ├── GameLoop.*        # Delta-time loop
│   ├── input/            # Command pattern
│   ├── physics/          # PhysicsEngine, Vector2
│   ├── state/            # Idle, Running, Jumping
│   ├── entity/           # Player, Entity
│   ├── renderer/         # Renderer, BackBuffer, Sprite
│   └── game/             # GameWorld (model)
├── CMakeLists.txt
└── README.md
```
