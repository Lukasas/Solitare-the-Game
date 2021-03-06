/**
  File: main.cpp
  Author: Lukáš Chábek (xchabe00)
  Description: Main file for creating a window.
  **/
#include "mainwindow.h"
#include <QApplication>

#include <time.h>
#include "game_board.h"
#include "carditem.h"
#include "sceneboard.h"

int main(int argc, char *argv[])
{
    srand(time(0));
    cGameBoard *board = new cGameBoard();
    board->GenerateNewGame();

    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}

