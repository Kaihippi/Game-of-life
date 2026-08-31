#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <SFML/Graphics.hpp>

// Pixel size (width and height) of a single cell when it is drawn to the
// SFML window. The window size is always a multiple of this value.
const int RECTANGLE_SIZE = 25;

// Title shown in the title bar of the simulation window.
const std::string GAME_NAME = "Game of life";

/**
 * Map
 *
 * Loads a Game of Life board from a text file, renders it with SFML, and
 * steps it forward in time using the standard Conway's Game of Life rules
 * (a live cell survives with 2 or 3 live neighbours, a dead cell is born
 * with exactly 3 live neighbours).
 *
 * The board is stored as a vector of equal-length strings, one string per
 * row, where each character is either '0' (dead) or any non-'0' character
 * (alive, conventionally '1').
 */
class Map {

public:

  /**
   * Loads the board from `filename`.
   *
   * The file is expected to contain one row per line, all lines the same
   * length, using '0' for dead cells and '1' for live cells. Blank lines
   * are skipped. Throws a `const char*` error message if the file cannot
   * be opened or if the rows are not all the same length.
   *
   * Also creates the SFML render window sized to fit the loaded board
   * (width = columns * RECTANGLE_SIZE, height = rows * RECTANGLE_SIZE).
   */
  Map(std::string filename);

  /**
   * Renders the current board state to the window: one filled rectangle
   * per cell. Dead cells are drawn black; live cells are drawn a random
   * RGB colour (regenerated every frame, purely decorative).
   */
  void Draw_map();

  /**
   * Advances the board by one generation using the classic Game of Life
   * neighbour-counting rule, treating cells outside the grid as simply
   * absent (no wraparound) - i.e. edge and corner cells have fewer
   * potential neighbours.
   */
  void Update_tiles();

  /**
   * Advances the board by one generation like Update_tiles(), but treats
   * the grid as toroidal: cells on one edge wrap around and are
   * considered neighbours of the cells on the opposite edge. This is the
   * version actually used by Map_loop().
   */
  void Update_tiles_2();

  /**
   * Main event/render loop for the simulation window. Runs until the
   * window is closed or Escape is pressed. While running:
   *   - "S" advances and redraws a single generation.
   *   - "Space" advances and redraws a single generation, then pauses
   *     briefly (~200ms) before the loop continues.
   *   - "E" runs a continuous auto-play loop (one generation every
   *     ~500ms) until "R" is pressed to stop it.
   */
  void Map_loop();

private:

  // Board state: one string per row, one character per cell
  // ('0' = dead, non-'0' = alive). All rows are the same length.
  std::vector<std::string> states_;

  // SFML window used to draw the board.
  sf::RenderWindow window_;

};
