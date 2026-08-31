# Game of Life

A small C++/SFML implementation of Conway's Game of Life, including:

- **Simulation viewer** — loads a board from a text file and steps it
  forward generation by generation on a toroidal (wrap-around) grid.
- **Map editor** — an interactive grid editor for creating and editing
  board files by hand, with a keyboard-driven cursor.

## Requirements

- A C++ compiler with `g++`
- [SFML](https://www.sfml.org/) (graphics, window, and system modules)

On Debian/Ubuntu:

```bash
sudo apt install libsfml-dev
```

## Build

```bash
make
```

This produces an executable called `main`. `make clean` removes the
intermediate `.o` files.

## Usage

```bash
./main <filename> <mode>
```

- `<mode> = Editor` — opens `<filename>` in the map editor. The file
  doesn't need to exist yet; it's created the first time you save.
- any other `<mode>` — loads `<filename>` as a board and runs the
  simulation.

Examples, using the sample boards in `maps/`:

```bash
./main maps/glider.txt run       # watch a glider fly across the grid
./main maps/blinker.txt run      # watch a simple 3-cell oscillator
./main maps/new_pattern.txt Editor   # design your own board
```

## Controls

### Simulation mode

| Key     | Action                                              |
|---------|------------------------------------------------------|
| Space   | Advance one generation                              |
| S       | Advance one generation                              |
| E       | Auto-play (steps every ~0.5s)                       |
| R       | Stop auto-play (while it's running)                 |
| Esc     | Close the window                                    |

### Editor mode

| Key          | Action                                  |
|--------------|------------------------------------------|
| Arrow keys   | Move the highlighted cursor tile        |
| Space        | Toggle the cell under the cursor         |
| W / S        | Grow the grid one column wider / one row taller |
| A / D        | Shrink the grid one column narrower / one row shorter |
| Esc          | Save and close                          |

Closing the editor window (in any way) saves the current board to the
given filename.

## Board file format

A board is a plain text file with one row per line. Each character in a
line is a single cell:

- `0` — dead
- any other character (conventionally `1`) — alive

All rows must be the same length. Blank lines are ignored. See
`maps/glider.txt` and `maps/blinker.txt` for examples.

## Project layout

```
main.cpp          Entry point / argument parsing
map.hpp/.cpp       Board loading, rendering, and the Game of Life rules
mapeditor.hpp/.cpp Interactive board editor
Makefile           Build rules
maps/              Sample board files
```
