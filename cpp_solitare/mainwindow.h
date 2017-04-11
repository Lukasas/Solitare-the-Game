#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "card.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Loading();
private:
};

#endif // MAINWINDOW_H
