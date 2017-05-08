#include "sceneboard.h"

#define CARD_OFFSET_Y 40

#define CARD_WIDTH 180
#define CARD_HEIGHT 261
#define BACK 53
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define LISTS_OFFSET_LEFT 100
#define X_OFFSET_BEGIN 20
#define Y_OFFSET_BEGIN 300
#define PACK_POS_X 20
#define PACK_POS_Y 20
#define FINAL_OFFSET_LEFT 600

#define CARD_BACK 53
#define PLACEHOLDER 0


/// Base cards must be named like this:
/// one_H.png (Heath) one_D.png (Diamond) one_S.png (Spade) one_C.png (Club)
QString BaseCardPaths[14] =
{
    "ace", // same for all colours
    "two", // same for all colours
    "three", // same for all colours
    "four", // same for all colours
    "five", // same for all colours
    "six", // same for all colours
    "seven", // same for all colours
    "eight", // same for all colours
    "nine", // same for all colours
    "ten", // same for all colours
    "jack", // same for all colours
    "queen", // same for all colours
    "king", // same for all colours
    "placeholder", // same for all colours
};

QString charBaseCardColor[4] =
{
    "_H",
    "_D",
    "_S",
    "_C"
};

QPixmap SceneBoard::GetCardImgById(int ID)
{
    return *(cards[ID]);
}

SceneBoard::SceneBoard()
{
    game = new cGameBoard();
    packCard = 0;
    LoadImages();
}

void SceneBoard::testGameEnded()
{
    if (game->gameEndedCheck())
    {
        ClearBoard();
        QFont f("Arial", 200);
        QGraphicsTextItem * finish = addText("Victory !", f);
        QRectF test = finish->boundingRect();
        finish->setPos((width()/2) - (test.width() / 2), height()/2 - (test.height() / 2));

   }
}

void SceneBoard::StartGame()
{
    ClearBoard();
    packCard = 0;
    game->GenerateNewGame();
    DrawLayout();
    FirstDrawTheGame();
}

bool SceneBoard::SaveGame(std::string GameName)
{
    return game->SaveGame(GameName);
}

void SceneBoard::StepBack()
{
    ClearBoard();
    game->MakeStepBack();
    DrawLayout();
    FirstDrawTheGame();
}

void SceneBoard::ClearBoard()
{
    clear();
}

bool SceneBoard::LoadGame(std::string GameName)
{    
    bool HappyLoad = game->LoadGame(GameName);
    if (!HappyLoad)
        return false;
    ClearBoard();
    DrawLayout();
    FirstDrawTheGame();
    return true;
}

void SceneBoard::PickNewCard()
{
    Card * c = game->GetPackCard();
    if (c == 0)
    {
        if (game->PickPackCard())
        {
            c = game->GetPackCard();
            packCard = new carditem(*c, this);
            packCard->setPos(PACK_POS_X * 2 + CARD_WIDTH, PACK_POS_Y);
            packCard->setPixmap(GetCardImgById(c->GetID()));
            addItem(packCard);
        }
    }
    else
    {
        if (game->PickPackCard())
        {
            c = game->GetPackCard();
            packCard->ChangeCard(c);
            packCard->setPixmap(GetCardImgById(c->GetID()));
        }
    }
}

carditem *SceneBoard::FindCardByName(std::string name)
{
    QList<QGraphicsItem*> all = items();
    for (int i = 0; i < all.count(); i++)
    {
        if (dynamic_cast<carditem*>(all.at(i)) != NULL)
        if (name.compare(((carditem*)all.at(i))->GetName()) == 0)
        {
            return ((carditem*)all.at(i));
        }
    }
    return 0;
}

void SceneBoard::MoveCard(std::string  Which, carditem *Where)
{
    carditem * movingCard = FindCardByName(Which);

    if (movingCard->Equal(*Where))
        return;

    if (packCard != 0 && Where->Equal(*packCard))
        return;

    CardPos wherePosition = game->GetCardLocation(Where);
    CardPos movingPosition = game->GetCardLocation(movingCard);
    if (movingPosition.ListID != -1) // Is in list
    {
        if (wherePosition.ListID != -1) // Is in list
        {
            std::vector<Card*> children = game->FindChildrenOfCard(movingPosition); // Needs to be here, becouse the position will change
            if (game->MoveCardToListFromList(movingPosition, wherePosition.ListID))
            {
                movingCard->setPos(Where->pos());
                movingCard->setY(Where->y() + CARD_OFFSET_Y);
                movingCard->setZValue(Where->zValue() + 1 );
                float yy_help = movingCard->y() + CARD_OFFSET_Y;
                for (int i = 0; i < children.size(); i++)
                {
                    carditem * kid = FindCardByName(((Card*)children.at(i))->GetName());
                    kid->setPos(movingCard->x(), yy_help);
                    kid->setZValue(movingCard->zValue() + (i + 1));
                    yy_help += CARD_OFFSET_Y;
                }
            }
        }
        else // Have to be Slot
        {
            if (movingCard->iGetCardColour() == Where->iGetCardColour())
            {
                if (packCard != 0 && Where->Equal(*packCard))
                    return;

                if (game->MoveCardToSlotFromList(movingPosition, Where->iGetCardColour()))
                {
                    movingCard->setPos(Where->pos());
                    movingCard->setY(Where->y());
                    movingCard->setZValue(Where->zValue() + 1 );
                }
            }
        }

    }
    else
    {
        if (movingCard->Equal(*packCard)) // From pack
        {
            if (wherePosition.ListID != -1) // to List !
            {
                if (game->MoveCardToListFromPack(wherePosition.ListID))
                {
                    movingCard->setPos(Where->pos());
                    movingCard->setY(Where->y() + CARD_OFFSET_Y);
                    movingCard->setZValue(Where->zValue() + 1 );
                    packCard = 0;
                    game->RemoveCardFromPack();
                }
            }
            else // From pack to Slot
            {
                if (movingCard->iGetCardColour() == Where->iGetCardColour())
                {
                    if (game->MoveCardToSlotFromPack(Where->iGetCardColour()))
                    {
                        movingCard->setPos(Where->pos());
                        movingCard->setY(Where->y());
                        movingCard->setZValue(Where->zValue() + 1 );
                        packCard = 0;
                        game->RemoveCardFromPack();
                    }
                }
            }

        }
        else // Have to be from Slot to List
        {
            if (wherePosition.ListID != -1) // Must be list otherwise ignore
            {
                if (game->MoveCardToListFromSlot(Where->iGetCardColour(), wherePosition.ListID))
                {
                    movingCard->setPos(Where->pos());
                    movingCard->setY(Where->y());
                    movingCard->setZValue(Where->zValue() + 1 );
                }
            }
        }
    }

    testGameEnded();
}

void SceneBoard::MoveCard(std::string Which, PlaceForKing *Where)
{
    carditem * movingCard = FindCardByName(Which);
    if (!movingCard->IsKing())
        return;
    CardPos movingPosition = game->GetCardLocation(movingCard);
    if (movingPosition.ListID != -1) // Is in list
    {
        std::vector<Card*> children = game->FindChildrenOfCard(movingPosition); // Needs to be here, becouse the position will change
        if (game->MoveCardToListFromList(movingPosition, Where->GetID()))
        {
            movingCard->setPos(Where->pos());
            movingCard->setY(Where->y());
            movingCard->setZValue(Where->zValue() + 1 );
            float yy_help = movingCard->y() + CARD_OFFSET_Y;
            for (int i = 0; i < children.size(); i++)
            {
                carditem * kid = FindCardByName(((Card*)children.at(i))->GetName());
                kid->setPos(movingCard->x(), yy_help);
                kid->setZValue(movingCard->zValue() + (i + 1));
                yy_help += CARD_OFFSET_Y;
            }
        }

    }
    else
    {
        if (packCard != 0 && movingCard->Equal(*packCard)) // From pack
        {
            if (game->MoveCardToListFromPack(Where->GetID()))
            {
                movingCard->setPos(Where->pos());
                movingCard->setY(Where->y());
                movingCard->setZValue(Where->zValue() + 1 );
                packCard = 0;
                game->RemoveCardFromPack();
            }
        }
        else // From slot
        {
            if (game->MoveCardToListFromSlot(movingCard->iGetCardColour(), Where->GetID()))
            {
                movingCard->setPos(Where->pos());
                movingCard->setY(Where->y());
                movingCard->setZValue(Where->zValue() + 1 );
            }
        }
    }
    testGameEnded();
}

void SceneBoard::MoveCard(std::string  Which, FinalPlace *Where)
{
    carditem * movingCard = FindCardByName(Which);
    if (!movingCard->IsAce() || movingCard->iGetCardColour() != Where->GetID())
        return;
    CardPos movingPosition = game->GetCardLocation(movingCard);
    if (movingPosition.ListID != -1) // Is in list
    {
        if (game->MoveCardToSlotFromList(movingPosition, Where->GetID()))
        {
            movingCard->setPos(Where->pos());
            movingCard->setY(Where->y());
            movingCard->setZValue(Where->zValue() + 1 );
        }
    }
    else
    {
        if (packCard != 0 && movingCard->Equal(*packCard)) // From pack
        {
            if (game->MoveCardToSlotFromPack(Where->GetID()))
            {
                movingCard->setPos(Where->pos());
                movingCard->setY(Where->y());
                movingCard->setZValue(Where->zValue() + 1 );
                packCard = 0;
                game->RemoveCardFromPack();
            }
        }
    }
    testGameEnded();
}

void SceneBoard::ShowCard(carditem *which)
{
    CardPos selected = game->GetCardLocation(which);
    if (game->CanBeShown(selected.ListID, selected.TopPos))
    {
        which->SetHidden(false);
        game->ShowCard(selected);
        which->setPixmap(GetCardImgById(which->GetID()));
    }
}

void SceneBoard::LoadImages()
{
    int currentID = 1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            QString test = ":/Resources/" + BaseCardPaths[j] + charBaseCardColor[i] + ".png";
            cards[currentID] = new QPixmap(QPixmap(test).scaled(CARD_WIDTH, CARD_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            if (cards[currentID]->width() == 0)
            {
                qDebug("Error!");
            }
            currentID++;
        }
    }
    cards[0] = new QPixmap(QPixmap(":/Resources/placeholder.png").scaled(CARD_WIDTH, CARD_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    cards[53] = new QPixmap(QPixmap(":/Resources/back.png").scaled(CARD_WIDTH, CARD_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void SceneBoard::DrawLayout()
{
    // RESOLUTION <= Control + F


    QGraphicsRectItem * rect = addRect(QRectF(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    rect->setZValue(0);

    PackClick = new CardPackClick(this);
    PackClick->setPixmap(*(cards[53]));
    PackClick->setPos(PACK_POS_X, PACK_POS_Y);


    QGraphicsPixmapItem *cardHolder = new QGraphicsPixmapItem();
    cardHolder->setPixmap(GetCardImgById(PLACEHOLDER));
    cardHolder->setPos(PACK_POS_X * 2 + CARD_WIDTH, PACK_POS_Y);

    addItem(PackClick);
    addItem(cardHolder);

    for (int i = 0; i < 7; i++)
    {
        PlaceForKing * item = new PlaceForKing(this, i);
        item->setZValue(0);
        item->setPos(X_OFFSET_BEGIN + i *(CARD_WIDTH + LISTS_OFFSET_LEFT),
                     Y_OFFSET_BEGIN);
        item->setPixmap(GetCardImgById(PLACEHOLDER));
        addItem(item);
    }

    for (int i = 0; i < 4; i++)
    {
        FinalPlace * item = new FinalPlace(this, i);
        item->setPos(X_OFFSET_BEGIN + FINAL_OFFSET_LEFT + i *(CARD_WIDTH + LISTS_OFFSET_LEFT),
                     PACK_POS_Y);
        item->setZValue(0);
        item->setPixmap(GetCardImgById(PLACEHOLDER));
        addItem(item);
    }
}

void SceneBoard::FirstDrawTheGame()
{
    for (int i = 0; i < 7; i++)
    {
        std::vector<Card*> list = game->GetCardList(i);
        for (int cardID = 0; cardID < list.size(); cardID++)
        {
            carditem * item = new carditem(*list[cardID], this);

            item->setPos(X_OFFSET_BEGIN + i*(CARD_WIDTH + LISTS_OFFSET_LEFT),
                         Y_OFFSET_BEGIN + cardID * CARD_OFFSET_Y);
            item->setZValue(cardID+1);
            if (!list[cardID]->IsHidden())
                item->setPixmap(*(cards[list[cardID]->GetID()]));
            else
                item->setPixmap(*(cards[53]));
            addItem(item);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        std::vector<Card*> Slot = game->GetCardSlot(i);
        for (int j = 0; j < Slot.size(); j++)
        {
            carditem * item = new carditem(*Slot[j], this);
            item->setPos(X_OFFSET_BEGIN + FINAL_OFFSET_LEFT + i *(CARD_WIDTH + LISTS_OFFSET_LEFT),
                         PACK_POS_Y);
            item->setZValue(j+1);
            if (!Slot[j]->IsHidden())
                item->setPixmap(*(cards[Slot[j]->GetID()]));
            else
                item->setPixmap(*(cards[53]));
            addItem(item);
        }
    }

    if (game->GetPackCard() != 0)
    {
        Card *c = game->GetPackCard();
        packCard = new carditem(*c, this);
        packCard->setPos(PACK_POS_X * 2 + CARD_WIDTH, PACK_POS_Y);
        packCard->setPixmap(GetCardImgById(c->GetID()));
        addItem(packCard);
    }
}



void CardPackClick::mousePressEvent(QGraphicsSceneMouseEvent*)
{

    board->PickNewCard();
}

void PlaceForKing::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    std::string name = event->mimeData()->text().toStdString();
    SceneBoard *b = (SceneBoard*)board;
    b->MoveCard(name, this);
}

PlaceForKing::PlaceForKing(SceneBoard *board, int ID)
{
    this->board = board;
    this->ID = ID;
    setAcceptDrops(true);
}

void FinalPlace::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    std::string name = event->mimeData()->text().toStdString();
    SceneBoard *b = (SceneBoard*)board;
    b->MoveCard(name, this);
}

FinalPlace::FinalPlace(SceneBoard *board, int ID)
{
    this->board = board;
    this->ID = ID;
    setAcceptDrops(true);
}
