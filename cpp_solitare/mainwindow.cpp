#include "mainwindow.h"
#include "card.h"
#include "carditem.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    view = new QGraphicsView(&scene_1);
    view->fitInView(scene_1.sceneRect(), Qt::KeepAspectRatio);
    //view->setAcceptDrops(true);
    setCentralWidget(view);
    scene_1.StartGame();
    view->fitInView(scene_1.sceneRect(), Qt::KeepAspectRatio);
}



MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent *)
{
    view->fitInView(scene_1.sceneRect(), Qt::KeepAspectRatio);
}
