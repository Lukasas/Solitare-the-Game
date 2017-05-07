/**
 * Module for command line gui implemented using ncurses library
 *
 * @author David Benes <xbenes41@stud.fit.vutbr.cz>
**/

#include <iostream>
#include <csignal>
#include <string>

#include <ncurses.h>
#include <unistd.h>

#define KEY_SPACE 32
#define Y_GAP 3
#define X_GAP 3

typedef struct arrow_movement {
    int pos[2];
    int card_edge_size;
} ARROW;

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

void card_back(WINDOW *card, int edge_size)
{
    wattron(card, COLOR_PAIR(1));
    for(int i = 1; i < edge_size; i++){
        for(int j = 1; j < edge_size; j++){
            mvwaddch(card, i, j, '?');
        }
    }
    wrefresh(card);
}

void put_card_values(WINDOW *card, int edge_size)
{
    std::string card_val = "♣ 10";
    wattron(card, COLOR_PAIR(2));
    mvwprintw(card, 1, 1, "%s", card_val.c_str());
    mvwprintw(card, edge_size - 2, edge_size - card_val.size() + 1, "%s", card_val.c_str());
    wrefresh(card);
}

void create_card_window(int start_y, int start_x, int card_edge_size)
{
    WINDOW *card = newwin(card_edge_size, card_edge_size, start_y, start_x);
    wborder(card, '|', '|', '-', '-', '+', '+', '+', '+');
    wbkgd(card, COLOR_PAIR(1));

    put_card_values(card, card_edge_size);
    wrefresh(card);
}

void draw_arrow(ARROW *arr_cfg, bool erase)
{
    // Define letters for arrow
    char left_side = (erase ? ' ' : '/');
    char right_side = (erase ? ' ' : '\\');

    int start_y = (Y_GAP + arr_cfg -> card_edge_size) * arr_cfg -> pos[0];
    int start_x = (X_GAP + (arr_cfg -> card_edge_size / 2 - 1)) + (X_GAP + arr_cfg -> card_edge_size) * arr_cfg -> pos[1];

    mvwaddch(stdscr, start_y, start_x, left_side);
    mvwaddch(stdscr, start_y, start_x + 1, right_side);
    mvwaddch(stdscr, start_y + 1, start_x - 1, left_side);
    mvwaddch(stdscr, start_y + 1, start_x + 2, right_side);
    wrefresh(stdscr);
}

void arrow_control(ARROW *arr_cfg)
{
    int kb_press;
    draw_arrow(arr_cfg, FALSE);
    while((kb_press = getch()) != KEY_F(10)){
        switch(kb_press){
            case KEY_UP:
                if(arr_cfg -> pos[0] == 1) break;
                if(arr_cfg -> pos[0] == 2 && arr_cfg -> pos[1] == 2)break;
                draw_arrow(arr_cfg, TRUE);
                arr_cfg -> pos[0]--;
                draw_arrow(arr_cfg, FALSE);
                break;
            case KEY_DOWN:
                if(arr_cfg -> pos[0] == 2) break;
                draw_arrow(arr_cfg, TRUE);
                arr_cfg -> pos[0]++;
                draw_arrow(arr_cfg, FALSE);
                break;
            case KEY_LEFT:
                if(arr_cfg -> pos[1] == 0) break;
                draw_arrow(arr_cfg, TRUE);
                if(arr_cfg -> pos[0] == 1 && arr_cfg -> pos[1] == 3)
                    arr_cfg -> pos[1] = 1;
                else
                    arr_cfg -> pos[1]--;
                draw_arrow(arr_cfg, FALSE);
                break;
            case KEY_RIGHT:
                if(arr_cfg -> pos[1] == 6) break;
                draw_arrow(arr_cfg, TRUE);
                if(arr_cfg -> pos[0] == 1 && arr_cfg -> pos[1] == 1)
                    arr_cfg -> pos[1] = 3;
                else
                    arr_cfg -> pos[1]++;
                draw_arrow(arr_cfg, FALSE);
                break;
            case KEY_SPACE:
                break;
        }
        wrefresh(stdscr);
    }
}

void init_playground(ARROW *arr_pos)
{
    int size_y, size_x;
    int x_shift = X_GAP;

    getmaxyx(stdscr, size_y, size_x);
    (void)size_y;

    if(size_x < 66){
        throw "Window is too small.";
    }

    int card_edge_size = (size_x > 94 ? 10 : 6 + (2 * ((size_x - 66) / 14)));
    arr_pos -> pos[0] = 1;
    arr_pos -> pos[1] = 0;
    arr_pos -> card_edge_size = card_edge_size;

    for(int i = 0; i < 7; i++){
        wmove(stdscr, 0, 0);
        // Create upper row of cards
        if(i != 2){
            create_card_window(Y_GAP, x_shift, card_edge_size);
        }
        // Create lower row of cards
        create_card_window(Y_GAP * 2 + card_edge_size, x_shift, card_edge_size);

        // Compute gap on x's axis for new card
        x_shift = x_shift + card_edge_size + 3;
    }
    arrow_control(arr_pos);
}

int main()
{
    std::setlocale(LC_ALL, "");
    signal(SIGWINCH, resizeterm);
    signal(SIGINT, keyboard_interrupt);

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);

    ARROW arrow_cfg;

    // Set window color
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_RED, COLOR_GREEN);
    wbkgd(stdscr, COLOR_PAIR(1));

    // init_playground();
    wrefresh(stdscr);
    try{
        init_playground(&arrow_cfg);
    }
    catch (const char *err_msg){
        endwin();
        std::cerr << err_msg << std::endl;
        exit(4);
    }

    endwin();
    return 0;
}
