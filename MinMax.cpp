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
// 出牌顺序：我方、敌人1、敌人2、我方……
const int8 our = 0;
const int8 enemy1 = 1;
const int8 enemy2 = 2;

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
    tmp[k] = x.currentPlayer == our ? '|' : (x.currentPlayer == enemy1 ? '{' : '}');
    for (int i = 0; i < N; ++i) {
        if (x.enemy1Cards.cardCount[i] == 0) continue;
        ++k;
        tmp[k] = (i << 3) + x.enemy1Cards.cardCount[i];
    }

    ++k;
    tmp[k] =  x.lastMoveOwner == our ? '|' : (x.lastMoveOwner == enemy1 ? '{' : '}');
    for (int i = 0; i < N; ++i) {
        if (x.enemy2Cards.cardCount[i] == 0) continue;
        ++k;
        tmp[k] = (i << 3) + x.enemy2Cards.cardCount[i];
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

    // 若当前玩家为我方的下家（敌人1）且我方牌已经出完
    if (x.currentPlayer == enemy1 && x.ourCards.cardNum() == 0) {
        caldCache.insert(id, maxEmptyResult);
        result = maxEmptyResult;
        return;
    }
    // 若当前玩家为敌人2且敌人1已经出完牌
    if (x.currentPlayer == enemy2 && x.enemy1Cards.cardNum() == 0) {
        caldCache.insert(id, minEmptyResult);
        result = minEmptyResult;
        return;
    }
    // 若当前玩家为我方且敌人2已经出完牌
    if (x.currentPlayer == our && x.enemy2Cards.cardNum() == 0) {
        caldCache.insert(id, minEmptyResult);
        result = minEmptyResult;
        return;
    }

    // 获取当前玩家可能出牌列表
    possibleMoveSet possibleMoves(x.currentPlayer == our ? x.ourCards :
                                                           (x.currentPlayer == enemy1 ? x.enemy1Cards :
                                                                                        x.enemy2Cards),
                                  x.lastMoveOwner == x.currentPlayer ? move() : x.lastMove);
    for (auto i = possibleMoves.moveSet.rbegin(); i != possibleMoves.moveSet.rend(); i++) {
        status y = x;
        if (x.currentPlayer == our) {
            y.ourCards.remove(i->mainCard);
            y.ourCards.remove(i->subCard);
            y.currentPlayer = enemy1;
        } else if (x.currentPlayer == enemy1) {
            y.enemy1Cards.remove(i->mainCard);
            y.enemy1Cards.remove(i->subCard);
            y.currentPlayer = enemy2;
        } else {
            y.enemy2Cards.remove(i->mainCard);
            y.enemy2Cards.remove(i->subCard);
            y.currentPlayer = our;
        }
        // y.lastMove = *i;//
        if (i->type != TYPE_0_PASS) {
            y.lastMove = *i;
            y.lastMoveOwner = x.currentPlayer;
        }

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
