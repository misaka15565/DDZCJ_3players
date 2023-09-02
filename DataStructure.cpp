#include <map>
#include <string>
#include <array>
#include "TypeDefines.hpp"
#include "DataStructure.hpp"

map<moveType, string> MOVE_TYPES_STR = {{TYPE_0_PASS, "过"},
                                        {TYPE_1_SINGLE, "单张"},
                                        {TYPE_2_PAIR, "对子"},
                                        {TYPE_3_TRIPLE, "三张"},
                                        {TYPE_4_BOMB, "炸弹"},
                                        {TYPE_5_KING_BOMB, "王炸"},
                                        {TYPE_6_3_1, "三带一"},
                                        {TYPE_7_3_2, "三带二"},
                                        {TYPE_8_SERIAL_SINGLE, "顺子"},
                                        {TYPE_9_SERIAL_PAIR, "连对"},
                                        {TYPE_10_SERIAL_TRIPLE, "飞机"},
                                        {TYPE_11_SERIAL_3_1, "三带一组成的飞机"},
                                        {TYPE_12_SERIAL_3_2, "三带二组成的飞机"},
                                        {TYPE_13_4_2, "四带二"},
                                        {TYPE_14_4_2_2, "四带两对"}};

map<char, cardVal> c2v = {
    {'3', 0},
    {'4', 1},
    {'5', 2},
    {'6', 3},
    {'7', 4},
    {'8', 5},
    {'9', 6},
    {'0', 7},
    {'J', 8},
    {'j', 8},
    {'Q', 9},
    {'q', 9},
    {'K', 10},
    {'k', 10},
    {'A', 11},
    {'a', 11},
    {'2', 13},
    {'X', 15},
    {'x', 15},
    {'D', 17},
    {'d', 17},
};

map<cardVal, string> v2s = {{0, "3"}, {1, "4"}, {2, "5"}, {3, "6"}, {4, "7"}, {5, "8"}, {6, "9"}, {7, "10"}, {8, "J"}, {9, "Q"}, {10, "K"}, {11, "A"}, {13, "2"}, {15, "小王"}, {17, "大王"}};

bool cards::isIncludedIn(cards b) const {
    for (int8 i = 0; i < N; i++)
        if (b.cardCount[i] > cardCount[i]) return false;
    return true;
}
bool cards::remove(cards b) {
    if (isIncludedIn(b) != true) return false;
    for (int8 i = 0; i < N; i++)
        cardCount[i] -= b.cardCount[i];
    return true;
}
cardVal cards::biggestCard() const {
    cardVal k = 0;
    for (int8 i = 0; i < N; i++) {
        if (cardCount[i] > 0 && i > k) k = i;
    }
    return k;
}
int8 cards::cardNum() const {
    int8 sum = 0;
    for (int8 i = 0; i < N; i++) {
        sum += cardCount[i];
    }
    return sum;
};
cards::cards() :
    cardCount{} {
}

bool cards::operator==(const cards &b) const {
    return cardCount == b.cardCount;
}
bool cards::operator<(const cards &b)const {
    for (int8 i = 0; i < N; i++) {
        if (cardCount[i] < b.cardCount[i])
            return true;
        else if (cardCount[i] > b.cardCount[i])
            return false;
    }
    return false;
}
move::move() {
    type = TYPE_0_PASS;
}
cards move::totalCards() const {
    cards sum = mainCard;
    for (int8 i = 0; i < N; i++) {
        sum.cardCount[i] += subCard.cardCount[i];
    }
    return sum;
};
int8 move::cardNum() const {
    return mainCard.cardNum() + subCard.cardNum();
};
statusCode move::isBiggerThan(move b) const {
    if (type == TYPE_0_PASS && b.type != TYPE_0_PASS) return bigger;
    if (type == TYPE_5_KING_BOMB) return bigger;
    if (type == TYPE_4_BOMB && b.type != TYPE_4_BOMB) return bigger;
    if (b.type == TYPE_5_KING_BOMB) return notbigger;
    if (b.type == TYPE_4_BOMB && type != TYPE_4_BOMB) return notbigger;
    if (type == b.type && cardNum() == b.cardNum()) {
        cardVal va = mainCard.biggestCard(), vb = b.mainCard.biggestCard();
        return va > vb ? bigger : notbigger;
    }
    return illegal;
};

bool operator==(const move &a, const move &b) {
    return a.mainCard == b.mainCard && a.subCard == b.subCard && a.type == b.type;
}