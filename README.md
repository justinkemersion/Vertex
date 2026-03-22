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
| **Model**  | GameWorld, Player, PhysicsEngine, Obstacles | Game state, physics, no I/O     |
| **View**   | Renderer, BackBuffer, Sprite             | Read positions, draw to terminal    |
| **Controller** | InputHandler, Commands              | Map keys to actions                 |

## Controls

| Key | Action |
|-----|--------|
| **← →** or **A** / **D** | Move left / right |
| **Space** or **↑** | Jump |
| **↓** or **S** | Stop horizontal movement (explicit) |
| **Q** | Quit |

**Terminal input:** There are no true key-up events in curses; the engine drains the input buffer each frame, combines **move + jump** when both appear in the same batch (so you can run and hop together), and stops movement after a short idle when no movement keys arrive (tuned for typical key-repeat timing). Gameplay while **holding forward and jumping** is still being refined.

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

**4. Run tests**

```bash
./bin/vertex_tests
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

## Testing

The project uses [doctest](https://github.com/doctest/doctest) for unit tests. Run `./bin/vertex_tests` after building.

| Suite | What it covers |
|-------|----------------|
| **PhysicsEngine** | Ground, ceiling, horizontal world bounds, gravity, obstacle landing vs side collision, jump-not-stopped while rising |
| **State transitions** | Idle ↔ Running, jump from idle, target velocity vs actual velocity (acceleration), air idle (target X zero in jump) |

---

## Requirements

- CMake 3.16+
- C++20 compiler (GCC 10+, Clang 10+, MSVC 2019+)
- ncurses (Linux/macOS) or PDCurses (Windows)

---

## Project layout

```
vertex/
├── .gitignore
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp              # Entry point
│   ├── Engine.*              # ncurses, input batching, subsystems
│   ├── GameLoop.*            # Fixed-step (~60 Hz) loop
│   ├── entity/
│   │   ├── Entity.hpp
│   │   └── Player.*
│   ├── game/
│   │   ├── GameWorld.*       # World size, camera, level/obstacles
│   │   └── Obstacle.hpp    # Axis-aligned obstacle blocks
│   ├── input/                # Command pattern (Command, InputHandler, moves, jump, idle)
│   ├── physics/
│   │   ├── PhysicsEngine.*
│   │   └── Vector2.hpp
│   ├── renderer/
│   │   ├── Renderer.*, BackBuffer.*, Sprite.hpp
│   └── state/                # PlayerState: Idle, Running, Jumping
└── tests/
    ├── PhysicsEngineTests.cpp
    └── StateTransitionTests.cpp
```

Build artifacts go under `build/` (ignored by git); binaries are written to `build/bin/`.
