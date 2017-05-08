/**
  File: mainwindow.h
  Author: Lukáš Chábek
  Description: Header file of mainwindow.cpp
  **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "sceneboard.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSignalMapper>
class MainWindow : public QMainWindow
{
    Q_OBJECT

    //------------------|
    //| 1      | 2      |
    //|        |        |
    //|-----------------|
    //| 3      | 4      |
    //|        |        |
    //|------------------


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void resizeEvent(QResizeEvent * event) override;
    /**
     * @brief AddGame
     * Adds a new window with game to view
     */
    void AddGame();
    /**
     * @brief RemoveGame
     * Removes window from game view
     */
    void RemoveGame();

    /**
     * @brief createActions
     * Create actions for menu
     */
    void createActions();
    /**
     * @brief createMenus
     * Creates context menu for main window
     */
    void createMenus();


    /**
     * @brief ResizeGames
     * Tries to adjust views of scenes
     */
    void ResizeGames();

    /**
     * @brief RefreshViewport
     * Does exactly same thing as ResizeGames
     */
    void RefreshViewport();

public slots:
    /**
     * @brief SaveGame
     * Saves game to file
     * @param gameID
     * ID of game (window)
     */
    void SaveGame(int gameID);

    /**
     * @brief LoadGame
     * Loads game from file
     * @param gameID
     * ID of game (window)
     */
    void LoadGame(int gameID);

    /**
     * @brief BackGame
     * Sends a notify to game to go back.
     * @param gameID
     * ID of game (window)
     */
    void BackGame(int gameID);

    /**
     * @brief NewGame
     * Creates a new game
     * @param gameID
     * ID of game (window)
     */
    void NewGame(int gameID);

    /**
     * @brief ChangeCountGames
     * Creates / Destroys games with their views
     * @param Num
     * Number of games to be active
     */
    void ChangeCountGames(int Num);
private:
    int countGames;

    QAction * acNew;
    QAction * GameSelect[4];

    QAction * NewGameAc[4];
    QAction * SaveGameAc[4];
    QAction * LoadGameAc[4];
    QAction * BackGameAc[4];

    QMenu * games[4];

    QMenu * gameMenu;
    QMenu * pickNum;
    QGridLayout * grid;
    SceneBoard scene[4];
    QGraphicsView *view[4];
};

#endif // MAINWINDOW_H
