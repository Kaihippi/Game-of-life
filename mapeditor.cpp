#include "mapeditor.hpp"


// Starts a fresh 1x1 dead-cell board and opens a matching 1-cell window.
// `filename` is stashed away for Save_map() to use later.
Map_editor::Map_editor(std::string filename)
  : filename_(filename), x_(0), y_(0) {

    std::string str = "0";
    states_.push_back(str);
    window_.create(sf::VideoMode(RECTANGLE_SIZE, RECTANGLE_SIZE), EDITOR_NAME);
    return;

  }


// Moves the cursor tile (x_, y_) by one cell in the given direction,
// clamped so it can never leave the grid.
//   1 = right, 2 = left, 3 = down, 4 = up.
void Map_editor::Move_chosen_tile(int num) {

  switch (num) {
    case 1:
      if(x_ < states_[0].length() - 1) {
        x_++;
      }
      break;
    case 2:
      if(x_ > 0) {
        x_--;
      }
      break;
    case 3:
      if(y_ < states_.size() - 1) {
        y_++;
      }
      break;
    case 4:
      if(y_ > 0) {
        y_--;
      }
      break;
    }

  return;

}


// Draws the board the same way Map::Draw_map() does (black = dead,
// random colour = alive), then overlays a white square on top of the
// cursor tile so the user can see which cell they're about to toggle.
void Map_editor::Draw_map() {

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

  // Cursor highlight, drawn last so it stays on top of the cell it
  // covers.
  sf::RectangleShape rectangle(sf::Vector2f(RECTANGLE_SIZE, RECTANGLE_SIZE));
  rectangle.setPosition(x_*RECTANGLE_SIZE, y_*RECTANGLE_SIZE);
  rectangle.setFillColor(sf::Color(255, 255, 255));
  window_.draw(rectangle);

  window_.display();

  return;

}


// Adds one dead ('0') cell to the end of every row, then resizes the
// window so it still fits the (now wider) grid exactly.
void Map_editor::Grow_map_x() {

  std::string str = "0";
  for(auto& state : states_) {
    state += str;
  }
  window_.create(sf::VideoMode(RECTANGLE_SIZE*states_[0].length(), RECTANGLE_SIZE*states_.size()), EDITOR_NAME);
  return;

}


// Adds one new all-dead row to the bottom of the grid (same width as
// the existing rows), then resizes the window to fit.
void Map_editor::Grow_map_y() {

  std::string str = "0";
  std::string row = "";
  for(auto chr : states_[0]) {
    row += str;
  }
  states_.push_back(row);
  window_.create(sf::VideoMode(RECTANGLE_SIZE*states_[0].length(), RECTANGLE_SIZE*states_.size()), EDITOR_NAME);
  return;

}


// Removes the rightmost column of the grid. Refuses to shrink below a
// width of 1. Resets the cursor to the top-left corner, since its old
// position may no longer be valid.
void Map_editor::Shrink_map_x() {

  x_ = 0;
  y_ = 0;

  if(states_[0].length() < 2) {
    return;
  }

  for(auto& str : states_) {
    str.erase(--(str.end()), str.end());
  }
  window_.create(sf::VideoMode(RECTANGLE_SIZE*states_[0].length(), RECTANGLE_SIZE*states_.size()), EDITOR_NAME);
  return;

}


// Removes the bottommost row of the grid. Refuses to shrink below a
// height of 1. Resets the cursor to the top-left corner, since its old
// position may no longer be valid.
void Map_editor::Shrink_map_y() {

  x_ = 0;
  y_ = 0;

  if(states_.size() < 2) {
    return;
  }

  states_.erase(--(states_.end()), states_.end());
  window_.create(sf::VideoMode(RECTANGLE_SIZE*states_[0].length(), RECTANGLE_SIZE*states_.size()), EDITOR_NAME);
  return;

}


// Flips the cell under the cursor between dead and alive.
void Map_editor::Change_state() {

  if(states_[y_][x_] == '0') {
    states_[y_][x_] = '1';
  } else {
    states_[y_][x_] = '0';
  }
  return;

}


// Writes the board to disk, one row per line, in the plain '0'/'1' text
// format that Map's constructor expects to read.
void Map_editor::Save_map() const {

  std::ofstream file;
  file.open(filename_);
  for(auto state : states_) {
    file << state <<  '\n';
  }
  file.close();
  return;

}


// Main input loop: handles resizing the grid, moving the cursor, and
// toggling cells, redrawing after every action. Saves the map to disk
// as soon as the window is closed (or Escape pressed).
void Map_editor::Editor_loop() {

  sf::Event event;

  while(window_.isOpen()) {
    window_.pollEvent(event);

    if(event.type == sf::Event::Closed) {
      window_.close();
      Save_map();
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window_.close();
      Save_map();
    }

    // W/S grow the grid (wider / taller); A/D shrink it (narrower /
    // shorter). Each is followed by a short pause so holding the key
    // down doesn't resize the grid many times per press.
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      Grow_map_x();
      Draw_map();
      sf::Clock clock;
      while(clock.getElapsedTime() < sf::microseconds(200000)) { continue; }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      Grow_map_y();
      Draw_map();
      sf::Clock clock;
      while(clock.getElapsedTime() < sf::microseconds(200000)) { continue; }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      Shrink_map_x();
      Draw_map();
      sf::Clock clock;
      while(clock.getElapsedTime() < sf::microseconds(200000)) { continue; }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      Shrink_map_y();
      Draw_map();
      sf::Clock clock;
      while(clock.getElapsedTime() < sf::microseconds(200000)) { continue; }
    }

    // Arrow keys move the highlighted cursor tile around the grid.
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      Move_chosen_tile(4);
      Draw_map();
      sf::Clock clock;
      while(clock.getElapsedTime() < sf::microseconds(200000)) { continue; }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      Move_chosen_tile(1);
      Draw_map();
      sf::Clock clock;
      while(clock.getElapsedTime() < sf::microseconds(200000)) { continue; }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      Move_chosen_tile(2);
      Draw_map();
      sf::Clock clock;
      while(clock.getElapsedTime() < sf::microseconds(200000)) { continue; }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      Move_chosen_tile(3);
      Draw_map();
      sf::Clock clock;
      while(clock.getElapsedTime() < sf::microseconds(200000)) { continue; }
    }

    // Space toggles the cell currently under the cursor.
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

      Change_state();
      Draw_map();
      sf::Clock clock;
      while(clock.getElapsedTime() < sf::microseconds(200000)) { continue; }

    }


  }

}
