#include "map.hpp"

// Reads the board from disk and sizes the window to match it.
Map::Map(std::string filename) {

  std::ifstream is;
  is.open(filename);

  if(is.fail()) {
    throw("Error in file reading");
  }

  // Read the file line by line. Every non-empty line becomes one row of
  // the board. As each row is appended we immediately check that it is
  // the same length as the first row, so a malformed file is rejected
  // as soon as the offending line is read.
  while(!is.eof()) {
    std::string line;
    getline(is, line);

    if(line.length() == 0) {
      continue;
    }

    states_.push_back(line);

    for(auto state : states_) {
      if(state.length() != states_[0].length()) {
        throw("Error in map size");
      }
    }

  }

  // Size the window so every cell gets exactly RECTANGLE_SIZE x
  // RECTANGLE_SIZE pixels: width = number of columns, height = number
  // of rows.
  int y = states_.size();
  int x = states_[0].length();
  window_.create(sf::VideoMode(x*RECTANGLE_SIZE, y*RECTANGLE_SIZE), GAME_NAME);

  return;

}


// Draws the current board: one rectangle per cell, black for dead cells
// and a random colour for live cells (the colour is re-rolled every call,
// so live cells flicker between colours each frame - purely visual, it
// has no effect on the simulation itself).
void Map::Draw_map() {

  window_.clear();

  for(int j = 0; j < states_.size(); j++) {
    for(int i = 0; i < states_[0].length(); i++) {

      sf::RectangleShape rectangle(sf::Vector2f(RECTANGLE_SIZE, RECTANGLE_SIZE));
      rectangle.setPosition(i*RECTANGLE_SIZE, j*RECTANGLE_SIZE);

      if(states_[j][i] == '0') {
        rectangle.setFillColor(sf::Color(0, 0, 0));
      } else {
        int a, b, c;
        a = std::rand()%255;
        b = std::rand()%255;
        c = std::rand()%255;
        rectangle.setFillColor(sf::Color(a, b, c));
      }

      window_.draw(rectangle);
    }
  }

  window_.display();

  return;

}


// Advances the board by one generation using a plain (non-wrapping)
// neighbourhood: for a cell at (i, j) we look at the up-to 3x3 block of
// cells around it, clamped to the grid edges, and count how many of
// those cells (including the cell itself) are alive. Subtracting is not
// needed because the classic rule is phrased in terms of "num" as
// "alive neighbours plus the cell's own state": a live cell needs a
// total count of 3 or 4 (i.e. 2 or 3 *neighbours*) to survive, and a
// dead cell needs a total count of exactly 3 (3 neighbours) to be born.
void Map::Update_tiles() {

  std::vector<std::string> new_vec;

  for(int j = 0; j < states_.size(); j++) {

    std::string new_str;
    for(int i = 0; i < states_[0].length(); i++) {

      // Count live cells in the 3x3 neighbourhood around (i, j),
      // clamped so we never read outside the vector (edge/corner
      // cells simply have fewer cells to look at, i.e. no wraparound).
      int num = 0;
      for(int k = std::max(0, j - 1); k < std::min((int)states_.size(), j + 2); k++) {
        for(int l = std::max(0, i - 1); l < std::min((int)states_[0].length(), i + 2); l++) {
          if(states_[k][l] != '0') {
            num++;
          }
        }
      }

      if(states_[j][i] == '1') {
        // "num" includes the cell itself, so 3 or 4 here means 2 or 3
        // live neighbours -> cell survives.
        if(num == 4 || num == 3) {
          new_str += "1";
        } else {
          new_str += "0";
        }
      } else {
        // Dead cell is born with exactly 3 live neighbours.
        if(num == 3) {
          new_str += "1";
        } else {
          new_str += "0";
        }
      }

    }

    new_vec.push_back(new_str);

  }

  states_ = new_vec;
  return;

}


// Same rule as Update_tiles(), but the grid wraps around at the edges
// (a torus): the row above row 0 is the last row, the column to the
// left of column 0 is the last column, and so on. This is the version
// actually called by Map_loop() during simulation.
void Map::Update_tiles_2() {

  std::cout << ((-1)%3) << std::endl;

  std::vector<std::string> new_vec;

  for(int j = 0; j < states_.size(); j++) {

    std::string new_str;
    for(int i = 0; i < states_[0].length(); i++) {

      int num = 0;

      // Build the list of the (up to) three row indices and three
      // column indices that make up this cell's 3x3 neighbourhood,
      // wrapping around to the opposite edge when j or i is at the
      // boundary of the grid.
      std::vector<int> num_y;
      std::vector<int> num_x;

      num_y.push_back(j);
      if(j == 0) {
        num_y.push_back(1);
        num_y.push_back(states_.size() - 1);
      } else if(j == states_.size() - 1) {
        num_y.push_back(states_.size() - 2);
        num_y.push_back(0);
      } else {
        num_y.push_back(j + 1);
        num_y.push_back(j - 1);
      }

      num_x.push_back(i);
      if(i == 0) {
        num_x.push_back(1);
        num_x.push_back(states_[0].length() - 1);
      } else if(i == states_[0].length() - 1) {
        num_x.push_back(states_[0].length() - 2);
        num_x.push_back(0);
      } else {
        num_x.push_back(i + 1);
        num_x.push_back(i - 1);
      }

      // Count live cells among the (row, column) combinations
      // gathered above - this is the toroidal 3x3 neighbourhood,
      // including the cell itself (same "num includes self" logic as
      // Update_tiles()).
      for(int l : num_y) {
        for(int k : num_x) {
          //std::cout << "l: " << l << " k: " << k << std::endl;
          if(states_[l][k] != '0') {
            num++;
            //std::cout << "Added number: y " << j << " x " << i << std::endl;
          }
        }
      }

      if(states_[j][i] == '1') {
        if(num == 4 || num == 3) {
          new_str += "1";
        } else {
          new_str += "0";
        }
      } else {
        if(num == 3) {
          new_str += "1";
        } else {
          new_str += "0";
        }
      }

    }

    new_vec.push_back(new_str);

  }

  states_ = new_vec;
  return;

}




// Main render/input loop for the simulation window.
void Map::Map_loop() {

  sf::Event event;

  while(window_.isOpen()) {
    window_.pollEvent(event);

    if(event.type == sf::Event::Closed) {
      window_.close();
    }

    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window_.close();
    }

    // "S": step forward one generation.
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      Update_tiles_2();
      Draw_map();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

      // "Space": step forward one generation, then wait ~200ms so a
      // single key press doesn't advance multiple generations while
      // the key is held down.
      Update_tiles_2();
      Draw_map();
      sf::Clock clock;
      while(clock.getElapsedTime() < sf::microseconds(200000)) { continue; }

    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {

      // "E": auto-play - keep stepping and redrawing every ~500ms
      // until "R" is pressed, then pause briefly before returning to
      // the outer event loop.
      bool quit = false;
      while(1) {

        if(quit) {
          sf::Clock clock1;
          while(clock1.getElapsedTime() < sf::microseconds(500000)) { continue; }
          break;
        }

        Update_tiles_2();
        Draw_map();
        sf::Clock clock2;
        while(clock2.getElapsedTime() < sf::microseconds(500000)) {
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            quit = true;
            break;
          }
          continue;
        }

      }

    }

  }

  return;

}
