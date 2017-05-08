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

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Loading();
    void resizeEvent(QResizeEvent * event) override;
    void AddGame();
    void RemoveGame();

    void createActions();
    void createMenus();

    void ResizeGames();
    void RefreshViewport();

public slots:
    void SaveGame(int gameID);
    void LoadGame(int gameID);
    void BackGame(int gameID);
    void NewGame(int gameID);
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
