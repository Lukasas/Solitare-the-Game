
CPPC = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -pedantic
LIBS = -lncursesw

cli_gui: cli_gui.cpp
	$(CPPC) $(CPPFLAGS) $< -o $@ $(LIBS)

clean:
	rm -rf cli_gui || true 2>/dev/null

.PHONY: clean cli_gui
