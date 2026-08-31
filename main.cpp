// Entry point.
//
// Usage:
//   ./main <filename> Editor    -> opens <filename> in the map editor
//                                   (see mapeditor.hpp/.cpp). The file
//                                   does not need to exist yet; it is
//                                   created when you save.
//   ./main <filename> <anything else>
//                               -> loads <filename> as a Game of Life
//                                   board (see map.hpp/.cpp) and runs
//                                   the simulation.
//
// Both modes require exactly two arguments after the program name.

#include "map.hpp"
#include "mapeditor.hpp"

int main(int argc, char** argv) {

  // Need at least a filename and a mode argument.
  if(argc == 0 || argc == 1) {
    return(0);
  }
  std::string filename = argv[1];
  std::string mode = argv[2];

  if(mode != "Editor") {

    // Simulation mode: load the board and run it. A malformed or
    // unreadable file causes Map's constructor to throw a message,
    // which we print and exit gracefully rather than crashing.
    try {
      Map map(filename);
      map.Draw_map();
      map.Map_loop();
      return(0);
    } catch(const char* msg) {
      std::cout << msg << std::endl;
      return(0);
    }

  } else {

    // Editor mode: open the interactive board editor for `filename`.
    Map_editor editor(filename);
    editor.Draw_map();
    editor.Editor_loop();
    return(0);

  }

}
