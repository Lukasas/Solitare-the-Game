/**
  File: scoreboard.h
  Author: Lukáš Chábek (xchabe00)
  Description: Header file of scoreboard.cpp
  **/

#ifndef SCENEBOARD_H
#define SCENEBOARD_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "card.h"
#include "carditem.h"
#include "game_board.h"

class SceneBoard;

class CardPackClick : public QGraphicsPixmapItem
{
private:
    SceneBoard * board;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
public:
    CardPackClick(SceneBoard * board) { this->board = board; }
};

class PlaceForKing : public QGraphicsPixmapItem
{
private:
    int ID;
    SceneBoard * board;
    void dropEvent(QGraphicsSceneDragDropEvent * event) override;
public:
    PlaceForKing(SceneBoard * board, int ID);
    int GetID() const { return ID; }
};

class FinalPlace : public QGraphicsPixmapItem
{
private:
    int ID;
    SceneBoard * board;
    void dropEvent(QGraphicsSceneDragDropEvent * event) override;
public:
    FinalPlace(SceneBoard * board, int ID);
    int GetID() const { return ID; }

};

class SceneBoard : public QGraphicsScene
{
private:

    cGameBoard * game;

    // 53 - Back | 0 - placeholder
    QPixmap *cards[54];

    carditem * packCard;

    CardPackClick * PackClick;

    QPixmap GetCardImgById(int ID);

    /**
     * @brief LoadImages
     * Loads all images for the scene
     */
    void LoadImages();

    /**
     * @brief DrawLayout
     * Draws basic layout for the game
     */
    void DrawLayout();

    /**
     * @brief FirstDrawTheGame
     * Redraws whole game from game data
     */
    void FirstDrawTheGame();
public:
    SceneBoard();
    /**
     * @brief testGameEnded
     * Checks if the game has ended
     */
    void testGameEnded();

    /**
     * @brief StartGame
     * Starts whole new game
     */
    void StartGame();

    /**
     * @brief LoadGame
     * Loads game from savefile
     * @param GameName
     * Path to the save file
     * @return
     * True if load was successfull
     */
    bool LoadGame(std::string GameName);

    /**
     * @brief SaveGame
     * Saves game to savefile
     * @param GameName
     * Path to the save file
     * @return
     * True if save was successfull
     */
    bool SaveGame(std::string GameName);

    /**
     * @brief StepBack
     * Make a Stap Back (basicly undo)
     */
    void StepBack();

    /**
     * @brief ClearBoard
     * Clears the whole board out of cards
     */
    void ClearBoard();

    /**
     * @brief PickNewCard
     * Picks a new card from pack
     */
    void PickNewCard();

    /**
     * @brief FindCardByName
     * Finds a card by it's name
     * @param name
     * Name of card
     * @return
     * Card item on the scene
     */
    carditem * FindCardByName(std::string name);

    /**
     * @brief MoveCard
     * Tries to move card to (List/Final Slots)
     * @param Which
     * Name of card to be moved
     * @param Where
     * Place where it sould be
     */
    void MoveCard(std::string  Which, carditem * Where);
    void MoveCard(std::string  Which, PlaceForKing * Where);
    void MoveCard(std::string  Which, FinalPlace * Where);

    /**
     * @brief ShowCard
     * Shows the card (Turns it)
     * @param which
     * Card on the battlefield
     */
    void ShowCard(carditem * which);

};

#endif // SCENEBOARD_H
