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

    game_play.GenerateNewGame();
    init_playground();
}

/**
 * @brief Destructor for CLI class
**/
CLI::~CLI(void)
{
    // Destroy all sub-windows
    for (int i = 0; i < 7; i++){
        delwin(game_area[2 + i].CARD_WINDOW);
        if(i != 2){
            delwin( game_area[1 - i].CARD_WINDOW);
        }
    }
    // End main window
    endwin();
}

void CLI::card_back(CARD_PLACEHOLDER window, int start_row, bool top)
{
    wattron(window.CARD_WINDOW, COLOR_PAIR(3));
    for(int i = start_row; i < (top ? window.height - 1 : start_row + 1); i++){
        for(int j = 1; j < window.width - 1; j++){
            mvwaddch(window.CARD_WINDOW, i, j, '?');
        }
    }
    wrefresh(window.CARD_WINDOW);
}

std::string CLI::get_card_value(Card *details)
{
    std::string tmp;
    switch (details -> iGetCardColour()) {
        case Heart:
            tmp = "♥ ";
            break;
        case Diamond:
            tmp = "♦ ";
            break;
        case Spade:
            tmp = "♠ ";
            break;
        case Club:
            tmp = "♣ ";
            break;
    }

    switch(details -> iGetCardValue()) {
        case CACE:
            tmp += "A";
            break;
        case CJ:
            tmp += "J";
            break;
        case CQ:
            tmp += "Q";
            break;
        case CK:
            tmp += "K";
            break;
        default:
            tmp += std::to_string(details -> iGetCardValue());
    }

    return tmp;
}

void CLI::put_card_values(CARD_PLACEHOLDER window)
{
    werase(window.CARD_WINDOW);
    int row = 0;
    if( ! window.CDList.empty())
        wresize(window.CARD_WINDOW, window.height + window.CDList.size() - 1, window.width);

    wbkgd(window.CARD_WINDOW, COLOR_PAIR(1));
    wattron(window.CARD_WINDOW, COLOR_PAIR(1));
    box(window.CARD_WINDOW, 0, 0);
    wrefresh(window.CARD_WINDOW);

    if(window.CDList.empty()) return;

    wbkgd(window.CARD_WINDOW, COLOR_PAIR(3));
    wattron(window.CARD_WINDOW, COLOR_PAIR(1));
    box(window.CARD_WINDOW, 0, 0);

    std::string card_val;
    for(std::vector<Card *>::iterator it = window.CDList.begin(); it != window.CDList.end(); ++it){
        row++;
        if((*it) -> IsHidden()){
            card_back(window, row, (it == window.CDList.end()));
            continue;
        }

        card_val = get_card_value(*it);
        wattron(window.CARD_WINDOW, ((*it) -> iGetCardColour() < 2 ? COLOR_PAIR(2) : COLOR_PAIR(3)));
        mvwprintw(window.CARD_WINDOW, row, 1, "%s", card_val.c_str());
        if(*it == window.CDList.front()){
            mvwprintw(window.CARD_WINDOW, (window.height - 2) + window.CDList.size() - (window.CDList.empty() ? 0 : 1), window.width - card_val.size() + 1, "%s", card_val.c_str());
        }

        if( ! window.expanded) break;
        card_val = "";
    }
    wrefresh(window.CARD_WINDOW);
}

WINDOW * CLI::create_card_window(int start_y, int start_x)
{
    WINDOW *card = newwin(card_edge_size, card_edge_size, start_y, start_x);
    wrefresh(card);
    return card;
}

void CLI::init_playground()
{
    int x_shift = x_gap;

    // Create sub-windows for cards
    for(int i = 0; i < 7; i++){
        CARD_PLACEHOLDER tmp;
        tmp.CARD_WINDOW = create_card_window(y_gap * 2 + card_edge_size, x_shift);
        tmp.CDList = game_play.GetCardList(i);
        tmp.height = card_edge_size;
        tmp.width = card_edge_size;
        tmp.expanded = TRUE;
        game_area[2 + i] = tmp;

        if(i != 2){
            CARD_PLACEHOLDER tmp;
            tmp.height = card_edge_size;
            tmp.width = card_edge_size;
            tmp.CARD_WINDOW = create_card_window(y_gap, x_shift);
            tmp.expanded = FALSE;
            game_area[1 - i] = tmp;
        }
        // Compute gap on x's axis for new card
        x_shift = x_shift + card_edge_size + 3;
    }

    update_screen();
}

void CLI::update_screen()
{
    for(int i = 0; i < 7; i++){
        put_card_values(game_area[2 + i]);
        if(i != 2){
            put_card_values(game_area[1 - i]);
        }
    }
}

void CLI::draw_arrow(bool erase)
{
    // Define letters for arrow
    char left_side = (erase ? ' ' : '/');
    char right_side = (erase ? ' ' : '\\');

    int win_height;
    if(arrow_pos[0] == 1){
        win_height = game_area[arrow_pos[0] - arrow_pos[1]].CDList.size();
    }
    else{
        win_height = game_area[arrow_pos[0] + arrow_pos[1]].CDList.size();
    }

    win_height -= (win_height > 0 ? 1 : 0);
    int start_y = ((y_gap + card_edge_size) * arrow_pos[0]) + win_height;
    int start_x = (x_gap + (card_edge_size / 2 - 1)) + (x_gap + card_edge_size) * arrow_pos[1];

    mvwaddch(stdscr, start_y, start_x, left_side);
    mvwaddch(stdscr, start_y, start_x + 1, right_side);
    mvwaddch(stdscr, start_y + 1, start_x - 1, left_side);
    mvwaddch(stdscr, start_y + 1, start_x + 2, right_side);
    wrefresh(stdscr);
}

void CLI::arrow_control()
{
    draw_arrow(FALSE);
    int kb_press;
    HOLDER tmp_card;
    bool picked_card = FALSE;
    while((kb_press = getch()) != KEY_F(10)){
        draw_arrow(FALSE);
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
                if( ! picked_card && game_area[arrow_pos[0] + (arrow_pos[0] == 1 ? (-arrow_pos[1]) : arrow_pos[1])].CDList.empty()){
                    break;
                }
                else if(arrow_pos[0] == 1 && arrow_pos[1] == 0){
                    if(game_area[1 - arrow_pos[1]].CDList.empty()){

                    }
                }
                else if (arrow_pos[0] != 1 && (arrow_pos[1] != 2 || arrow_pos[1] != 0) && ! picked_card){
                    if(game_area[arrow_pos[0] + (arrow_pos[0] == 1 ? (-arrow_pos[1]) : arrow_pos[1])].CDList.back() -> IsHidden()){
                        game_area[arrow_pos[0] + (arrow_pos[0] == 1 ? (-arrow_pos[1]) : arrow_pos[1])].CDList.back() -> SetHidden(FALSE);
                    }
                    else if ( ! game_area[arrow_pos[0] + (arrow_pos[0] == 1 ? (-arrow_pos[1]) : arrow_pos[1])].CDList.empty()){
                        tmp_card.old_y = arrow_pos[0];
                        tmp_card.old_x = arrow_pos[1];
                        tmp_card.card = game_area[arrow_pos[0] + (arrow_pos[0] == 1 ? (-arrow_pos[1]) : arrow_pos[1])].CDList.back();
                        game_area[arrow_pos[0] + (arrow_pos[0] == 1 ? (-arrow_pos[1]) : arrow_pos[1])].CDList.pop_back();
                    }
                }
                else if (picked_card){
                    game_area[tmp_card.old_y + (tmp_card.old_y == 1 ? (-tmp_card.old_x) : tmp_card.old_x)].CDList.push_back(tmp_card.card);
                }
                picked_card = (picked_card ? FALSE : TRUE);
                break;
        }
        update_screen();
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
