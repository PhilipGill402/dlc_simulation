# Digital Logic Circuit Simulation 

## DESCRIPTION
This simulation is an interactive graphical simulation for building and testing simple
digital logic circuits. Users can place logic gates, toggle input sources, and
connect components together using wires to observe real-time signal propagation.

The simulation is designed as a learning and experimentation tool, emphasizing
clarity of behavior, visual feedback, and intuitive mouse-driven interaction.

---

## OVERVIEW

The simulation window consists of:

- Input sources (toggleable)
- Logic gates (AND, OR, NOT, etc.)
- Wires connecting outputs to inputs
- Real-time visual feedback for signal state

---

## Running

To compile and run just run:
```bash
make && ./sim
```

## INTERFACE

### Gates
- And gates are colored green
- Or gates are colored blue
- Nor gates are colored red

---

### Screenshot: Empty Simulation
<!-- SCREENSHOT: initial empty simulation window -->
![Empty simulation window](screenshots/empty_simulation.png)

---

### Screenshot: Gates and Inputs Placed
<!-- SCREENSHOT: gates and inputs placed -->
![Gates and inputs](screenshots/gates.png)

---

### Screenshot: Active Wires (On State)
<!-- SCREENSHOT: wires showing active/high signal -->
![Active wires](screenshots/wires.png)

---

## Saving

The simulation currently supports saving your current state and reloading it. The program uses a JSON file to hold the current state and this state is loaded when called. To save the state just press ```s``` and to load the saved state just press ```v```. Currently you can only have one save state at a time.

---

## CONTROLS

### Mouse Controls

| Action | Description |
|------|-------------|
| Left Click (Input) | Toggle input on/off |
| Left Click + Drag (Gate) | Move gate |
| Left Click + Drag (Pin) | Create wire |
| Release on Input Pin | Connect wire |
| Release elsewhere | Cancel wire |

---

### Keyboard Controls

| Key | Action |
|---|---|
| `R` | Reset simulation |
| `A` | Create an And gate |
| `O` | Create an Or gate |
| `N` | Create a Nor gate |
| `I` | Create an Input node |
| `L` | Create a Light |
| `S` | Save the state of the simulation |
| `V` | Load the saved state of the simulation |

---

## WIRING BEHAVIOR

- Wires connect **output pins** to **input pins**
- Each wire propagates a boolean signal
- Active (HIGH) wires are drawn in a different color than inactive (LOW) wires
- Wires are rendered orthogonally using an L-shaped path

---

## LOGIC EVALUATION

Each simulation frame:

1. Input values are updated
2. Wire signals propagate to gate inputs
3. Gates evaluate their output
4. Visual state is updated

This ensures deterministic and predictable behavior.

---

## FILES

- `main.cpp` — application entry point
- `simulation.*` — simulation state and update logic
- `gate.*` — logic gate implementations
- `wire.*` — wire geometry and connections
- `input.*` — input source handling
- `light.*` - light handling
- `constants.h` - contains constants used throughout the program

---

## BUGS

Known limitations may include:

- No wire deletion (yet)
- Limited gate types

Report issues by opening a GitHub issue.

---

## AUTHOR
Written by *Philip Gill*

---
