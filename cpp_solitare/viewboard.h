#ifndef VIEWBOARD_H
#define VIEWBOARD_H

#include <QGraphicsView>
#include <QGraphicsSceneEvent>
class ViewBoard : public QGraphicsView
{
private:
    void dragEnterEvent(QDragEnterEvent *event) override;
public:
    ViewBoard();
    ViewBoard::ViewBoard(QGraphicsScene * scene);
};

#endif // VIEWBOARD_H
