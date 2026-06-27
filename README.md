# linefollrobo 🤖

An autonomous line-following robot built during my internship at IVW Private Limited.  
Navigates a black-line track to a user-specified node (1–8), stops at the destination, and autonomously returns to the start.

---

## What it does

- User sends a target node number (1–8) via Serial/GUI
- Robot follows a black line using 3 IR sensors
- Counts intersections (full-black nodes) to track position
- Executes pre-programmed left turns at specific nodes (4 & 5)
- On reaching the target: beeps, performs a 180° U-turn, and returns home
- Buzzer provides audio feedback at each stage

---

## Hardware

| Component | Details |
|---|---|
| Microcontroller | Arduino (Uno/Nano) |
| IR Sensors | 3-channel (Left A0, Middle A1, Right A2) — HIGH = black |
| Motor Driver | L298N dual H-bridge |
| Motors | 2× DC gear motors |
| Buzzer | Pin 12 |
| Communication | Serial (9600 baud) via USB / GUI |

---

## How it works

### Sensor pattern → action (3-bit encoding)

| Pattern (L M R) | Action |
|---|---|
| `010` | Move forward (on line) |
| `110` / `100` | Veer right (line drifted left) |
| `011` / `001` | Veer left (line drifted right) |
| `111` | **Node detected** — count + turn logic |
| `000` | Lost line — recover using last known direction |

### Node navigation

- Each `111` (full black) intersection increments `nodeCount`
- Nodes 4 and 5 are programmed as left-turn nodes (`leftTurnNode[]` array)
- On reaching `targetNode`: executes 180° right spin (~650 ms), then returns
- On returning to start (`nodeCount == targetNode` while `returning = true`): stops and resets

---

## State machine

```
WAIT FOR COMMAND
      ↓
FOLLOW LINE → detect node → increment counter
      ↓                          ↓
  keep going            left turn? (node 4/5)
                                 ↓
                      reached target? → U-TURN → return home
                                                      ↓
                                               reached start → STOP + RESET
```

---

## Serial commands

| Input | Action |
|---|---|
| `1` – `8` | Set target node and start |

Output logs position, turns, destination reached, and return events.

---

## Key parameters to tune per chassis

```cpp
delay(500);   // Left turn duration   — line 55
delay(650);   // 180° U-turn duration — line 73
delay(30);    // Main loop cycle time — line 105
```

---

## Results

- Navigated all 8 nodes reliably across straight and intersection track layouts  
- Zero line-loss events in final internship demo  
- U-turn and return path functioned correctly across 3 test runs

---

## What I'd improve next

- Replace 3-sensor array with 5-channel for smoother PID-based steering  
- Add encoder feedback to replace time-based turn logic (more reliable across surfaces)  
- Replace hardcoded `leftTurnNode[]` with runtime path upload via Serial  
- Add RFID tags at each node for absolute position confirmation (eliminates miscounts on missed nodes)
