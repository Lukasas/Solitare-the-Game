/**
 * Module for command line gui implemented using ncurses library
 *
 * @author David Benes <xbenes41@stud.fit.vutbr.cz>
**/

#include <iostream>
#include <csignal>
#include <string>
#include <queue>
#include <map>

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "game_board.h"
#include "cli_gui.h"

void keyboard_interrupt(int signum)
{
    endwin();
    std::cerr << "Keyboard Interrupt. Game Over!" << std::endl;
    exit(signum);
}

void resizeterm(int signum)
{
    (void)signum;
    int y,x;
    getmaxyx(stdscr, y, x);
    printw("x: %d, y: %d", x, y);
    wmove(stdscr, 0, 0);
    wrefresh(stdscr);
    return;
}

/**
 * @brief Constructor for CLI class
**/
CLI::CLI(void)
{
    std::setlocale(LC_ALL, "");
    srand(time(0));

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN); // Font: white, BG: green
    init_pair(2, COLOR_RED, COLOR_WHITE);   // Font: red, BG: white
    init_pair(3, COLOR_BLACK, COLOR_WHITE); // Font: black BG: white
    wbkgd(stdscr, COLOR_PAIR(1));
    wrefresh(stdscr);

    y_gap = 3;
    x_gap = 3;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    (void) max_y;
    if(max_x < 66){
        throw "Window is too small.";
    }

    card_edge_size = (max_x > 94 ? 10 : 6 + (2 * ((max_x - 66) / 14)));
    arrow_pos[0] = 1;
    arrow_pos[1] = 0;

    init_playground();
}

/**
 * @brief Destructor for CLI class
**/
CLI::~CLI(void)
{
    // Destroy all sub-windows
    CARD_SPOT tmp;
    for (int i = 0; i < 7; i++){
        tmp = game_area[1 - i];

        while( ! tmp.empty()){
            delwin(tmp.back().CARD_WINDOW);
            tmp.pop();
        }

        if(i != 2){
            tmp = game_area[2 + i];

            while( ! tmp.empty()){
                delwin(tmp.back().CARD_WINDOW);
                tmp.pop();
            }
        }
    }

    // End main window
    endwin();
}

void CLI::card_back(PLACEHOLDER window)
{
    wattron(window.CARD_WINDOW, COLOR_PAIR(3));
    for(int i = 1; i < window.window_width - 1; i++){
        for(int j = 1; j < window.window_height - 1; j++){
            mvwaddch(window.CARD_WINDOW, i, j, '?');
        }
    }
    wrefresh(window.CARD_WINDOW);
}

void CLI::put_card_values(PLACEHOLDER window)
{
    wbkgd(window.CARD_WINDOW, COLOR_PAIR(1));
    wattron(window.CARD_WINDOW, COLOR_PAIR(1));
    box(window.CARD_WINDOW, 0, 0);
    wrefresh(window.CARD_WINDOW);

    if(window.blank){
        return;
    }
    else if(window.secret){
        card_back(window);
        return;
    }

    wbkgd(window.CARD_WINDOW, COLOR_PAIR(3));
    wattron(window.CARD_WINDOW, COLOR_PAIR(1));
    box(window.CARD_WINDOW, 0, 0);

    int ran_num = rand() % 9 + 2;

    std::string card_val = "♣ " + std::to_string(ran_num);
    wattron(window.CARD_WINDOW, COLOR_PAIR(2));
    mvwprintw(window.CARD_WINDOW, 1, 1, "%s", card_val.c_str());
    mvwprintw(window.CARD_WINDOW, window.window_width - 2, window.window_width - card_val.size() + 1, "%s", card_val.c_str());
    wrefresh(window.CARD_WINDOW);
}

WINDOW * CLI::create_card_window(int start_y, int start_x)
{
    WINDOW *card = newwin(card_edge_size, card_edge_size, start_y, start_x);
    wrefresh(card);
    return card;
}

void CLI::draw_arrow(bool erase)
{
    // Define letters for arrow
    char left_side = (erase ? ' ' : '/');
    char right_side = (erase ? ' ' : '\\');

    int start_y = (y_gap + card_edge_size) * arrow_pos[0];
    int start_x = (x_gap + (card_edge_size / 2 - 1)) + (x_gap + card_edge_size) * arrow_pos[1];

    mvwaddch(stdscr, start_y, start_x, left_side);
    mvwaddch(stdscr, start_y, start_x + 1, right_side);
    mvwaddch(stdscr, start_y + 1, start_x - 1, left_side);
    mvwaddch(stdscr, start_y + 1, start_x + 2, right_side);
    wrefresh(stdscr);
}

void CLI::arrow_control()
{
    bool picked_card = FALSE;

    draw_arrow(FALSE);
    int kb_press;
    while((kb_press = getch()) != KEY_F(10)){
        switch(kb_press){
            case KEY_UP:
                if(arrow_pos[0] == 1) break;
                if(arrow_pos[0] == 2 && arrow_pos[1] == 2)break;
                draw_arrow(TRUE);
                arrow_pos[0]--;
                draw_arrow(FALSE);
                break;
            case KEY_DOWN:
                if(arrow_pos[0] == 2) break;
                draw_arrow(TRUE);
                arrow_pos[0]++;
                draw_arrow(FALSE);
                break;
            case KEY_LEFT:
                if(arrow_pos[1] == 0) break;
                draw_arrow(TRUE);
                if(arrow_pos[0] == 1 && arrow_pos[1] == 3)
                    arrow_pos[1] = 1;
                else
                    arrow_pos[1]--;
                draw_arrow(FALSE);
                break;
            case KEY_RIGHT:
                if(arrow_pos[1] == 6) break;
                draw_arrow(TRUE);
                if(arrow_pos[0] == 1 && arrow_pos[1] == 1)
                    arrow_pos[1] = 3;
                else
                    arrow_pos[1]++;
                draw_arrow(FALSE);
                break;
            case KEY_SPACE:
                picked_card = (picked_card ? FALSE : TRUE);
                break;
        }
        wrefresh(stdscr);
    }
}

void CLI::init_playground()
{
    int x_shift = x_gap;

    for(int i = 0; i < 7; i++){
        PLACEHOLDER tmp;
        tmp.window_height = card_edge_size;
        tmp.window_width = card_edge_size;
        tmp.CARD_WINDOW = create_card_window(y_gap * 2 + card_edge_size, x_shift);
        tmp.blank = FALSE;
        tmp.secret = FALSE;
        game_area[1 - i].push(tmp);

        if(i != 2){
            PLACEHOLDER tmp;
            tmp.window_height = card_edge_size;
            tmp.window_width = card_edge_size;
            tmp.CARD_WINDOW = create_card_window(y_gap, x_shift);
            tmp.blank = (i > 2);
            tmp.secret = (i == 0);
            game_area[2 + i].push(tmp);
        }
        // Compute gap on x's axis for new card
        x_shift = x_shift + card_edge_size + 3;
    }

    for(int i = 0; i < 7; i++){
        PLACEHOLDER tmp = game_area[1 - i].back();
        put_card_values(tmp);
        if(i != 2){
            tmp = game_area[2 + i].back();
            put_card_values(tmp);
        }
    }
}

int main()
{
    // Signal handlers
    signal(SIGINT, keyboard_interrupt);
    signal(SIGWINCH, resizeterm);

    try{
        CLI cli_gui;
        cli_gui.arrow_control();
    }
    catch (const char *err_msg){
        endwin();
        std::cerr << err_msg << std::endl;
        exit(4);
    }

    return 0;
}
