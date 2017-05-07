#include "carditem.h"

void carditem::dropEvent(QGraphicsSceneDragDropEvent * event)
{
    event->accept();
    qDebug(event->mimeData()->text().toStdString().c_str());
    QList<QGraphicsItem*> all = scene()->items();
    for (int i = 0; i < all.count(); i++)
    {

        if (dynamic_cast<carditem*>(all.at(i)) != NULL)
        if (strcmp(event->mimeData()->text().toStdString().c_str(), ((carditem*)all.at(i))->name) == 0)
        {
            ((carditem*)all.at(i))->hide();

        }
    }
}

void carditem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{

}

void carditem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{

}

void carditem::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
    setAcceptDrops(true);
}

void carditem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug(name);
}

void carditem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QDrag *drag = new QDrag((QObject*)event->widget());
    QMimeData *mime = new QMimeData;
    drag->setMimeData(mime);
    mime->setText(name);
    mime->setImageData(pixmap().toImage());
    drag->setPixmap(pixmap().scaled(pixmap().size()));

    drag->exec();
}

carditem::carditem(char * name) : Card(5)
{
    this->name = name;
    setAcceptDrops(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    //setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}
