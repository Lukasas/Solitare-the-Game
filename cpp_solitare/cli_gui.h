#ifndef _CLI_GUI_H_
#define _CLI_GUI_H_

#include <string>
#include <vector>
#include <queue>
#include <map>

#include <ncurses.h>

#include "game_board.h"

#define KEY_SPACE 32

typedef struct placeholder_details{
    WINDOW *CARD_WINDOW;
    std::vector<Card*> CDList;
    int height;
    int width;
    bool expanded;
} CARD_PLACEHOLDER;

typedef std::map<int, CARD_PLACEHOLDER> PLAYGROUND;

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
        cGameBoard game_play;
        PLAYGROUND game_area;
        int arrow_pos[2];

        // Methods
        void init_playground();
        WINDOW * create_card_window(int start_y, int start_x);
        std::string get_card_value(Card details);
        void update_screen();
        void card_back(CARD_PLACEHOLDER window);
        void draw_arrow(bool erase);
};

#endif
