
CPPC = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -pedantic
LIBS = -lncursesw

cli_gui: cli_gui.o card.o game_board.o
	$(CPPC) $(CPPFLAGS) $^ -o $@ $(LIBS)

cli_gui.o: cli_gui.cpp cli_gui.h
	$(CPPC) $(CPPFLAGS) -c $< -o $@ $(LIBS)

card.o: card.cpp card.h
	$(CPPC) $(CPPFLAGS) -c $< -o $@ $(LIBS)

game_board.o: game_board.cpp game_board.h
	$(CPPC) $(CPPFLAGS) -c $< -o $@ $(LIBS)

clean:
	rm -rf *.o || true 2>/dev/null
	rm -rf cli_gui || true 2>/dev/null

.PHONY: clean cli_gui
