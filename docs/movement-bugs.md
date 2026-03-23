# Movement Bug Analysis & Fixes

## Symptoms

1. **Jolting/stuttering movement** — player moves but jerks intermittently while holding a direction key
2. **Legs do not animate** — the run sprite never visibly alternates between stride frames
3. **Non-uniform forward momentum during jumps** — horizontal movement while airborne feels inconsistent and buggy on landing

---

## Root Causes

### 1. Auto-idle threshold too aggressive (primary cause of all three symptoms)

**File:** `src/Engine.cpp` (processInput), `src/Engine.hpp`

The engine infers "key released" by counting frames without a key event, since ncurses has no key-up events. The held-key threshold was **4 frames (~67ms at 60fps)**, but ncurses key repeat typically delivers events at ~30Hz (~33ms apart). A natural 2-3 frame gap between repeat events was enough to trigger the auto-idle, which:

- Fired a full `IdleCommand`, causing a **state transition** from `RunningState` → `IdleState`
- Zeroed `target_velocity_x_`, causing a brief deceleration
- On the next frame, a key event would arrive and transition back to `RunningState`

This rapid flickering between Idle and Running states caused:
- **Jolts**: brief deceleration + instant re-acceleration = sawtooth velocity pattern
- **No leg animation**: during Idle frames the idle sprite (identical to run frame 0) was shown, disrupting the alternation between stride poses
- **Jump landing bugs**: stale `movement_held_` state and interaction with the grounded check

### 2. Auto-idle caused state transitions (amplified all symptoms)

**File:** `src/Engine.cpp`

The auto-idle called `IdleCommand().execute(*player)`, which dispatched through the state machine and triggered a full state transition to `IdleState`. This was far more disruptive than necessary — the intent was just to stop the player, not to change their animation state.

### 3. Ground friction far too low

**File:** `src/entity/Player.hpp`

`GROUND_FRICTION = 100.0f` with `RUN_SPEED = 85.0f` meant the player took **~0.85 seconds** to decelerate to a stop. This created:
- An ice-skating feel after releasing keys
- Made the auto-idle jolts more visible (asymmetric accel at 1200 vs decel at 100)

### 4. Landing transition used exact float comparison

**File:** `src/state/JumpingState.cpp`

```cpp
if (player.getVelocityX() != 0.0f)  // exact float comparison
```

This checked **actual velocity** with `!= 0.0f`, which could misfire due to floating-point imprecision. It also checked instantaneous velocity rather than player intent (target velocity), leading to wrong state transitions on landing.

---

## Fixes Applied

### Fix 1: Time-based auto-idle with higher thresholds

**Files:** `src/Engine.hpp`, `src/Engine.cpp`, `src/GameLoop.cpp`

- Converted frame counter (`int frames_since_movement_`) to time accumulator (`float time_since_movement_`)
- Increased held-key timeout from ~67ms to **200ms** (`HELD_IDLE_TIME = 0.20f`)
- Tap timeout set to **350ms** (`TAP_IDLE_TIME = 0.35f`)
- `processInput` now receives `dt` from the game loop for accurate time tracking
- 200ms is well above the typical key repeat interval (~33ms), eliminating false "key released" detection

### Fix 2: Auto-idle only zeroes target velocity, no state transition

**File:** `src/Engine.cpp`

Instead of `IdleCommand().execute(*player)` (which triggered a full state transition), the auto-idle now calls `player->setTargetVelocityX(0.0f)` directly. The player stays in `RunningState` while decelerating, keeping the run animation active and avoiding sprite flickering.

### Fix 3: RunningState naturally transitions to Idle when stopped

**File:** `src/state/RunningState.cpp`

Added a check in `RunningState::update()`: when both `target_velocity_x` and `velocity_x` are near zero, transition to `IdleState`. This handles the natural deceleration-to-stop case that was previously handled by the (now removed) state-transitioning auto-idle.

```cpp
if (std::abs(player.getTargetVelocityX()) < 0.1f &&
    std::abs(player.getVelocityX()) < 1.0f) {
    player.setState(std::make_unique<IdleState>());
}
```

### Fix 4: Landing transition uses target velocity with threshold

**File:** `src/state/JumpingState.cpp`

Changed the landing check from `getVelocityX() != 0.0f` to `std::abs(getTargetVelocityX()) > 0.1f`. This bases the landing state on **player intent** (are they holding a direction key?) rather than instantaneous velocity, and uses a threshold instead of exact float comparison.

### Fix 5: Ground friction increased to 800

**File:** `src/entity/Player.hpp`

`GROUND_FRICTION`: 100 → **800**. Deceleration time from run speed to stop: ~0.85s → **~0.1s**. This gives a responsive, snappy stop rather than an ice-skating slide.

---

## Tuning Reference

All tuning constants live in `src/entity/Player.hpp`:

| Constant | Value | Effect |
|---|---|---|
| `GRAVITY` | 460.0 | Downward acceleration (units/s²) |
| `RUN_SPEED` | 85.0 | Target horizontal speed (units/s) |
| `RUN_ACCELERATION` | 1200.0 | How fast the player reaches run speed (units/s²) |
| `GROUND_FRICTION` | 800.0 | Deceleration rate when stopping (units/s²) |
| `AIR_CONTROL` | 1.0 | Multiplier on horizontal accel while airborne (1.0 = full control) |
| `JUMP_VELOCITY` | -120.0 | Initial upward velocity on jump (negative = up) |

Auto-idle thresholds live in `src/Engine.hpp`:

| Constant | Value | Effect |
|---|---|---|
| `HELD_IDLE_TIME` | 0.20 | Seconds without key event before inferring key release (held keys) |
| `TAP_IDLE_TIME` | 0.35 | Seconds without key event before inferring key release (first press) |

If movement still feels off, the most impactful values to adjust are:
- `HELD_IDLE_TIME` — raise if keys still cause jolts on your system (keyboard repeat rate varies)
- `GROUND_FRICTION` — lower for more slide, higher for snappier stops
- `RUN_ACCELERATION` — lower for more gradual startup
