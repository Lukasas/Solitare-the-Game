#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "sceneboard.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Loading();
    void resizeEvent(QResizeEvent * event) override;
private:
    SceneBoard scene_1;
    SceneBoard scene_2;
    SceneBoard scene_3;
    SceneBoard scene_4;
    QGraphicsView *view;
};

#endif // MAINWINDOW_H
