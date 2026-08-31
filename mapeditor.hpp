#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "map.hpp"

// Title shown in the title bar of the editor window.
const std::string EDITOR_NAME = "Editor";

/**
 * Map_editor
 *
 * A small interactive editor for creating/editing Game of Life board
 * files. Starts from a single dead cell and lets the user grow or
 * shrink the grid, move a highlighted cursor tile around, toggle cells
 * alive/dead, and save the result to disk in the same text format that
 * Map (see map.hpp) reads.
 */
class Map_editor {

public:

  /**
   * Starts a new editor session for `filename`. The board always starts
   * as a single dead cell (1x1); it is not loaded from `filename` even
   * if that file already exists. `filename` is only used later, by
   * Save_map(), as the destination to write to.
   */
  Map_editor(std::string filename);

  /**
   * Moves the highlighted cursor tile by one cell, clamped to stay
   * within the current grid bounds. `num` selects the direction:
   *   1 = right, 2 = left, 3 = down, 4 = up.
   */
  void Move_chosen_tile(int num);

  /**
   * Renders the current board: one rectangle per cell (black for dead,
   * random colour for alive, same as Map::Draw_map()), plus a white
   * rectangle drawn on top to highlight the cursor tile at (x_, y_).
   */
  void Draw_map();

  /** Appends a new dead ('0') column to the right edge of every row and
   *  resizes the window to match. */
  void Grow_map_x();

  /** Appends a new dead ('0') row to the bottom of the grid and resizes
   *  the window to match. */
  void Grow_map_y();

  /** Removes the rightmost column (no-op if the grid is only 1 cell
   *  wide) and resets the cursor to (0, 0). */
  void Shrink_map_x();

  /** Removes the bottommost row (no-op if the grid is only 1 cell tall)
   *  and resets the cursor to (0, 0). */
  void Shrink_map_y();

  /** Toggles the cell currently under the cursor between alive and
   *  dead. */
  void Change_state();

  /** Writes the current board to `filename_`, one row per line, in the
   *  same '0'/'1' text format Map expects to load. */
  void Save_map() const;

  /**
   * Main event/input loop for the editor window. Runs until the window
   * is closed or Escape is pressed (both save the map on exit):
   *   - W / S: grow the grid one column wider / one row taller.
   *   - A / D: shrink the grid one column narrower / one row shorter.
   *   - Arrow keys: move the cursor tile up/down/left/right.
   *   - Space: toggle the cell under the cursor.
   * Each action is followed by a short (~200ms) pause so a single key
   * press doesn't repeat the action many times while held down.
   */
  void Editor_loop();


private:

  // Board state: one string per row, one character per cell
  // ('0' = dead, non-'0' = alive). All rows are the same length.
  std::vector<std::string> states_;

  // Destination file that Save_map() writes to.
  std::string filename_;

  // SFML window used to draw the board and cursor.
  sf::RenderWindow window_;

  // Column/row of the currently highlighted (editable) cell.
  int x_;
  int y_;

};
