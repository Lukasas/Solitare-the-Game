#include "viewboard.h"

void ViewBoard::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug("DRAG VIEW ");
}

ViewBoard::ViewBoard()
{

}

ViewBoard::ViewBoard(QGraphicsScene * scene)
{
    QGraphicsScene::QGraphicsScene(scene);
}
