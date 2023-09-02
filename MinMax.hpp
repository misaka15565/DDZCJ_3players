#ifndef MINMAX_HPP
#define MINMAX_HPP
#include "DataStructure.hpp"
extern const int8 maxScore;
extern const int8 minScore;
extern const int8 our;
extern const int8 enemy1;
extern const int8 enemy2;
struct returned_result {
    int score;
    move bestMove;
};
struct status {
    cards ourCards;
    cards enemy1Cards;
    cards enemy2Cards;
    move lastMove;
    int8 currentPlayer;
    int8 lastMoveOwner;
};
returned_result minMaxSearch(const status &x);
#endif