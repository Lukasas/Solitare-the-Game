#ifndef _CLI_GUI_H_
#define _CLI_GUI_H_

#include <string>
#include <queue>
#include <map>

#include <ncurses.h>

#include "game_board.h"

#define KEY_SPACE 32

typedef struct card_details{
    std::string value;
    bool turned;
} CARD;

typedef std::queue<CARD> CSTACK;

typedef struct placeholder_details{
    WINDOW *CARD_WINDOW;
    int window_height;
    int window_width;
    bool blank;
    bool secret;
} CARD_PLACEHOLDER;

typedef std::queue<CARD_PLACEHOLDER> CARD_SPOT;
typedef std::map<int, CARD_SPOT> PLAYGROUND;

class CLI{
    public:
        // Variables
        int card_edge_size;
        int y_gap, x_gap;

        // Methods
        CLI(); // Constructor
        ~CLI(); // Destructor
        void put_card_values(CARD_PLACEHOLDER window);
        void arrow_control();

    private:
        // Variables
        PLAYGROUND game_area;
        int arrow_pos[2];

        // Methods
        void init_playground();
        WINDOW * create_card_window(int start_y, int start_x);
        void card_back(CARD_PLACEHOLDER window);
        void draw_arrow(bool erase);
};

#endif
