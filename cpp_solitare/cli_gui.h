#ifndef _CLI_GUI_H_
#define _CLI_GUI_H_

#include <string>
#include <vector>
#include <queue>
#include <map>

#include <ncurses.h>

#include "game_board.h"

/// Bind space key
#define KEY_SPACE 32

/// Struct for teporary storing of holded card
typedef struct picked_card{
    Card *card;
    int old_y;
    int old_x;
} HOLDER;

/// Struct for detail about sub-windows
typedef struct placeholder_details{
    WINDOW *CARD_WINDOW;
    std::vector<Card*> CDList;
    int height;
    int width;
    bool expanded;
} CARD_PLACEHOLDER;

/// Map of sub-windows
typedef std::map<int, CARD_PLACEHOLDER> PLAYGROUND;

class CLI{
    public:
        // Variables
        int card_edge_size; /// Edge of card based on main window size
        int y_gap, x_gap;   /// Gap between cards or cards and window shape

        // Methods
        CLI(); /// Constructor
        ~CLI(); /// Destructor

        /**
         * @brief Print value of top card in a pack or sequence of values in pack
         *
         * @param window Sub-window parameters
        **/
        void put_card_values(CARD_PLACEHOLDER window);

        /**
         * @brief Game control. This method controls all game
        **/
        void arrow_control();

    private:
        // Variables
        cGameBoard game_play;   /// Instance with created game
        PLAYGROUND game_area;   /// Information about game area
        int arrow_pos[2];       /// Position of pointing arrow in the game area

        // Methods
        /**
         * @brief Initialize game area.
         * Generate new game and print out game scene
        **/
        void init_playground();

        /**
         * @brief Create sub-window for cards
         *
         * @param start_y Position on axis y where window will be created
         * @param start_x Position on axis x where window will be created
         *
         * @return Pointer to new created window
        **/
        WINDOW * create_card_window(int start_y, int start_x);

        /**
         * @brief Create card value string based on data retreived from Card class instance
         *
         * @param details Card class instance
         *
         * @return String containing card value for print into card
        **/
        std::string get_card_value(Card *details);

        /**
         * @brief Print out back side of a card
         *
         * @param window Window parameters
         * @param start_row Where on the axis y we want to start printing
         * @param top Detecent if card is on top or below top.
        **/
        void card_back(CARD_PLACEHOLDER window, int start_row, bool top);

        /**
         * @brief Draw pointing arrow based on coordinates stored as class attribute
         *
         * @param erase TRUE if we want erase arrow on given coordinates or FALSE if we want to draw it.
        **/
        void draw_arrow(bool erase);

        /**
         * @brief This will rewrite whole game scene. It's called every keyboard event
        **/
        void update_screen();
};

#endif
