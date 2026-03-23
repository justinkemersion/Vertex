# How Movement Works (and How to Make It Yours)

This guide walks through the code that controls how the player moves, jumps, accelerates, and stops. Every value mentioned here is a plain number sitting in a source file — change it, rebuild, and see what happens. That's the whole point.

---

## Where the numbers live

Open `src/entity/Player.hpp` and look near the top of the class. You'll see a block of `static constexpr float` lines. These are **constants** — named values that the rest of the code reads but never changes at runtime. They're the dials for how the game feels.

```cpp
static constexpr float GRAVITY         = 460.0f;
static constexpr float RUN_SPEED       = 85.0f;
static constexpr float RUN_ACCELERATION = 1200.0f;
static constexpr float GROUND_FRICTION = 800.0f;
static constexpr float AIR_CONTROL     = 1.0f;
static constexpr float JUMP_VELOCITY   = -120.0f;
```

Each one does exactly what the name says. Here's what you'll feel when you change them:

| Constant | What it controls | Try this |
|---|---|---|
| `GRAVITY` | How fast the player falls | Lower it (200) for floaty moon-jumps. Raise it (800) for snappy, heavy drops. |
| `RUN_SPEED` | Top horizontal speed | Crank it to 200 and watch the world blur past. Drop it to 30 for a cautious walk. |
| `RUN_ACCELERATION` | How quickly you reach top speed | Set it equal to `RUN_SPEED` and you'll feel a slow wind-up. Set it to 5000 and movement is instant. |
| `GROUND_FRICTION` | How quickly you stop after releasing a key | Low values (100) feel like ice. High values (2000) feel like glue. |
| `AIR_CONTROL` | How much you can steer mid-jump (0.0 to 1.0) | Set it to 0 — once you jump, you're committed. Set it to 0.5 for partial steering. |
| `JUMP_VELOCITY` | How high you jump (negative = upward) | -80 is a small hop. -200 launches you off-screen. |

After changing any of these, rebuild and run:

```bash
cd build && cmake --build . && ./bin/vertex
```

---

## How pressing a key becomes movement

This is the chain of events that happens every frame (~60 times per second). Each step lives in a different file, and each file focuses on one job. Following this chain is a good way to learn how C++ projects are organized.

### 1. The game loop ticks (`src/GameLoop.cpp`)

The loop measures how much real time passed since the last frame (`dt`, short for "delta time") and passes it to everything else. Every speed and acceleration is multiplied by `dt` so the game runs the same regardless of how fast your computer is.

**Concept spotlight:** `dt` is a `float` variable — a number with a decimal point. It's usually around 0.016 (1/60th of a second). You'll see it passed as a function argument everywhere.

### 2. Input is read (`src/Engine.cpp` — `processInput`)

The engine drains all key presses that arrived since the last frame using `getch()` in a `while` loop. It sorts them into categories: horizontal movement, jump, or idle.

**Concept spotlight:** The `while` loop here reads from a *buffer* — a queue of key events the terminal saved up. This is an array under the hood, managed by ncurses. The engine pulls items out one at a time until the buffer is empty (`ERR`).

### 3. Commands translate keys into actions (`src/input/`)

Each key maps to a **Command** object (`MoveRightCommand`, `JumpCommand`, etc.). The command calls `player.getState()->handleInput(player, *this)`, which hands the action to whatever state the player is currently in.

**Concept spotlight:** This is the **Command Pattern**. Each command is its own small class with an `execute` function. The `#include` at the top of each command file pulls in the class definition it needs — that's what an include/import does: it copies the contents of another file so the compiler knows what `Player` or `PlayerState` looks like.

### 4. States decide what happens (`src/state/`)

The player is always in exactly one state: **Idle**, **Running**, or **Jumping**. Each state is a separate `.cpp` file with its own `handleInput` and `update` functions.

- `IdleState` — waiting. A move command transitions to Running. A jump command transitions to Jumping.
- `RunningState` — moving. Advances the leg animation each frame. Transitions to Idle when velocity reaches zero, or to Jumping on a jump command.
- `JumpingState` — airborne. Preserves horizontal momentum. On landing, picks Running or Idle based on whether you're still holding a direction key.

**Concept spotlight:** This is the **State Pattern**. Instead of a giant `if/else` checking "am I jumping? am I running?", each state is its own class. The player holds a `std::unique_ptr<PlayerState>` — a smart pointer that owns one state object at a time and automatically cleans up the old one when switching.

### 5. Physics makes it real (`src/physics/PhysicsEngine.cpp`)

The physics engine doesn't know about keys or states. It just sees numbers: current velocity, target velocity, gravity, friction. It smoothly accelerates `velocity_x` toward `target_velocity_x` using the acceleration and friction constants from Player. It applies gravity to `velocity_y`. Then it moves the player's position and checks for collisions with the ground and obstacles.

**Concept spotlight:** The obstacles are stored in a `std::vector<Obstacle>` — a dynamic array that can grow or shrink. The physics loops through this vector with a `for` loop to check each obstacle for collision. `Vector2` (in `src/physics/Vector2.hpp`) is a simple struct with just two floats: `x` and `y`. It's a building block used everywhere positions appear.

### 6. Rendering draws the frame (`src/renderer/`)

The renderer reads the player's position and sprite, subtracts the camera offset, and writes ASCII characters into a **back buffer** — a 2D grid of characters (`std::vector<std::string>`). Then it copies the whole buffer to the terminal in one shot. This prevents flickering.

---

## The auto-idle problem (and why terminals are tricky)

Terminals don't tell you when a key is *released* — they only tell you when a key is *pressed*. When you hold a key, the operating system generates repeated press events, but there are natural gaps between them. The engine has to guess when you actually let go.

This guessing is handled by a timer in `src/Engine.cpp`. If no movement key arrives for long enough, the engine assumes you released the key and starts decelerating.

The timing thresholds live in `src/Engine.hpp`:

| Constant | Value | What it means |
|---|---|---|
| `HELD_IDLE_TIME` | 0.20s | After confirming a key is held (repeat events arriving), wait this long without an event before assuming release |
| `TAP_IDLE_TIME` | 0.55s | On a fresh key press (before repeat is confirmed), wait this long — covers the initial keyboard repeat delay |
| `MAX_REPEAT_GAP` | 0.15s | If two key events arrive within this gap, the key is confirmed as "held" |

If movement feels like it pauses or stutters on your machine, your keyboard's repeat delay might be different. Check it:

```bash
xset q | grep delay
```

If your delay is, say, 600ms, raise `TAP_IDLE_TIME` above 0.6 in `src/Engine.hpp`.

---

## Quick experiments

Here are some things to try that each touch a different part of the code:

1. **Moon gravity** — In `Player.hpp`, set `GRAVITY = 100.0f` and `JUMP_VELOCITY = -60.0f`. Watch the jump arc change completely.

2. **Ice level** — Set `GROUND_FRICTION = 50.0f`. You'll slide for a long time after releasing the key. Pair with `RUN_ACCELERATION = 200.0f` for a slow wind-up.

3. **No air control** — Set `AIR_CONTROL = 0.0f`. Once you jump, you can't change direction. This forces you to commit to your jumps (classic arcade feel).

4. **Speed run** — Set `RUN_SPEED = 200.0f` and `RUN_ACCELERATION = 5000.0f`. Instant top speed. See if you can still clear the obstacles.

5. **New obstacle** — In `src/game/GameWorld.cpp`, find the `buildLevel` function. Add a line to the obstacles vector. Each obstacle is just `{x, y, width, height}`.

6. **Change the sprite** — In `src/entity/Player.cpp`, look at the constructor. The player's appearance is just three lines of text in `sprite_idle_`, `sprite_run_1_`, and `sprite_run_2_`. Change the characters and you've got a new character.

---

## File map

If you want to explore further, here's where everything lives and what C++ concepts each file demonstrates:

| File | What it does | Concepts you'll see |
|---|---|---|
| `src/entity/Player.hpp` | Tuning constants, class definition | `static constexpr`, member variables, smart pointers |
| `src/entity/Player.cpp` | Sprite definitions, animation logic | Constructors, `std::vector<std::string>`, `dynamic_cast` |
| `src/state/*.cpp` | State machine (Idle, Running, Jumping) | Inheritance, virtual functions, `std::make_unique` |
| `src/input/*.cpp` | Command pattern (keys to actions) | Polymorphism, `#include`, small single-purpose classes |
| `src/physics/PhysicsEngine.cpp` | Velocity, gravity, collisions | `float` math, `for` loops over vectors, `std::clamp` |
| `src/Engine.cpp` | Game orchestration, input timing | `getch()`, `while` loops, time accumulation |
| `src/GameLoop.cpp` | Frame timing (~60 fps) | `std::chrono`, `sleep_for`, delta time |
| `src/renderer/Renderer.cpp` | ASCII drawing, back buffering | 2D array (vector of strings), camera offset |
| `src/game/GameWorld.cpp` | Level layout, obstacles | `std::vector<Obstacle>`, struct initialization |
