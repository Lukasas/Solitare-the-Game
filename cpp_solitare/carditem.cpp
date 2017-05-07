#include "carditem.h"
#include "sceneboard.h"
void carditem::dropEvent(QGraphicsSceneDragDropEvent * event)
{

    std::string name = event->mimeData()->text().toStdString();
    SceneBoard *b = (SceneBoard*)Scene;
    b->MoveCard(name, this);
}

void carditem::dragEnterEvent(QGraphicsSceneDragDropEvent *)
{
}

void carditem::dragLeaveEvent(QGraphicsSceneDragDropEvent *)
{

}

void carditem::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
    setAcceptDrops(true);
}

void carditem::mousePressEvent(QGraphicsSceneMouseEvent *)
{

}

void carditem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    SceneBoard * b = (SceneBoard*)Scene;
    b->ShowCard(this);
}

void carditem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!IsHidden())
    {
        QDrag *drag = new QDrag((QObject*)event->widget());
        QMimeData *mime = new QMimeData;
        drag->setMimeData(mime);
        mime->setText(GetName().c_str());
        mime->setImageData(pixmap().toImage());
        drag->setPixmap(pixmap().scaled(pixmap().size()));
        drag->exec();
    }
}

carditem::carditem(Card & C, void * c) : Card(C)
{
    Scene = c;
    setAcceptDrops(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    //setFlag(QGraphicsItem::ItemIsSelectable);
    /*setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);*/
}

void carditem::ChangeCard(Card *C)
{
    SetCardValue(C->iGetCardValue());
    SetCardColour(C->iGetCardColour());
    SetName(C->GetName());
}
