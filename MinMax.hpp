#ifndef MINMAX_HPP
#define MINMAX_HPP
#include "DataStructure.hpp"
extern const int8 maxScore;
extern const int8 minScore;
extern const int8 our;
extern const int8 enemy;

struct returned_result {
    int score;
    move bestMove;
};
struct status {
    cards ourCards;
    cards enemyCards;
    move lastMove;
    int8 currentPlayer;
};
returned_result minMaxSearch(const status &x);
#endif