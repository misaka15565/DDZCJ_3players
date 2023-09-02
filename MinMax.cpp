#include "MoveGener.hpp"
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <functional>
#include <string>
#include <unordered_map>
#include <thread>
#include <vector>
#include "easyMap.hpp"
#include "MinMax.hpp"

const int8 maxScore = 100;
const int8 minScore = 0;
const int8 our = 0;
const int8 enemy = 1;

string status2id(const status &x) {
    string id;
    char tmp[100] = {0};

    int k = -1;
    for (int i = 0; i < N; ++i) {
        if (x.ourCards.cardCount[i] == 0) continue;
        ++k;
        tmp[k] = (i << 3) + x.ourCards.cardCount[i];
    }

    ++k;
    tmp[k] = x.currentPlayer == our ? '{' : '}';
    for (int i = 0; i < N; ++i) {
        if (x.enemyCards.cardCount[i] == 0) continue;
        ++k;
        tmp[k] = (i << 3) + x.enemyCards.cardCount[i];
    }

    ++k;
    tmp[k] = '|';
    for (int i = 0; i < N; ++i) {
        if (x.lastMove.mainCard.cardCount[i] == 0) continue;
        ++k;
        tmp[k] = (i << 3) + x.lastMove.mainCard.cardCount[i];
    }
    if (tmp[k] == '|') {
        tmp[k] = 0;
        id = tmp;
        return id;
    }

    ++k;
    tmp[k] = '|';
    for (int i = 0; i < N; ++i) {
        if (x.lastMove.subCard.cardCount[i] == 0) continue;
        ++k;
        tmp[k] = (i << 3) + x.lastMove.subCard.cardCount[i];
    }
    if (tmp[k] == '|') {
        tmp[k] = 0;
        id = tmp;
        return id;
    }

    id = tmp;
    return id;
}

easyMAP<string, returned_result> caldCache;
void buildMap(status x, returned_result &result) {
    auto id = status2id(x);
    returned_result res2;
    auto flag = caldCache.find(id, res2);
    if (flag == true) {
        result = res2;
        return;
    }
    move tmp;
    tmp.type = TYPE_0_PASS;
    returned_result minEmptyResult = {minScore, tmp};
    returned_result maxEmptyResult = {maxScore, tmp};

    if (x.currentPlayer == enemy && x.ourCards.cardNum() == 0) {
        caldCache.insert(id, maxEmptyResult);
        result = maxEmptyResult;
        return;
    }
    if (x.currentPlayer == our && x.enemyCards.cardNum() == 0) {
        caldCache.insert(id, minEmptyResult);
        result = minEmptyResult;
        return;
    }

    possibleMoveSet possibleMoves(x.currentPlayer == our ? x.ourCards : x.enemyCards, x.lastMove);
    for (auto i = possibleMoves.moveSet.rbegin(); i != possibleMoves.moveSet.rend(); i++) {
        status y = x;
        if (x.currentPlayer == our) {
            y.ourCards.remove(i->mainCard);
            y.ourCards.remove(i->subCard);
            y.currentPlayer = enemy;
        } else {
            y.enemyCards.remove(i->mainCard);
            y.enemyCards.remove(i->subCard);
            y.currentPlayer = our;
        }
        y.lastMove = *i;

        buildMap(y, res2);
        if (x.currentPlayer == our) {
            if (res2.score == maxScore) {
                caldCache.insert(id, {maxScore, *i});
                result = {maxScore, *i};
                return;
            }
        } else {
            if (res2.score == minScore) {
                caldCache.insert(id, {minScore, *i});
                result = {minScore, *i};
                return;
            }
        }
    }
    if (x.currentPlayer == our) {
        caldCache.insert(id, minEmptyResult);
        result = minEmptyResult;
        return;
    } else {
        caldCache.insert(id, maxEmptyResult);
        result = maxEmptyResult;
        return;
    }
}

returned_result minMaxSearch(const status &x) {
    returned_result result;
    if (caldCache.isEmpty()) buildMap(x, result);
    auto id = status2id(x);
    caldCache.find(id, result);
    return result;
}
