# Builds the "main" executable from the three translation units below,
# linking against SFML's graphics/window/system modules.
#
# Usage:
#   make        - build ./main
#   make clean  - remove build artifacts (*.o)

main: main.o mapeditor.o map.o
	g++ -o main main.o mapeditor.o map.o -lsfml-graphics -lsfml-window -lsfml-system

main.o:	main.cpp
	g++ -c main.cpp

map.o: map.cpp
	g++ -c map.cpp

mapeditor.o:	mapeditor.cpp
	g++ -c mapeditor.cpp

clean:
	rm -f *.o
