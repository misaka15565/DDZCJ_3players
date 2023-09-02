#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP
#include <map>
#include <string>
#include <array>
#include "TypeDefines.hpp"

using std::string;
using std::map;
using std::array;

extern map<moveType, string> MOVE_TYPES_STR;
extern map<char, cardVal> c2v;
extern map<cardVal, string> v2s;
#define N 18
class cards {
public:
    array<cardVal, N> cardCount; // CardValue,CardCount
    bool isIncludedIn(cards b) const;
    bool remove(cards b);
    cardVal biggestCard() const;
    int8 cardNum() const;
    cards();
    bool operator==(const cards &b) const;
    bool operator<(const cards &b) const;
};
bool operator<(const cards &a, const cards &b);
class move {
public:
    cards mainCard;
    cards subCard;
    moveType type;
    move();
    cards totalCards() const;
    int8 cardNum() const;
    statusCode isBiggerThan(move b) const;
};

bool operator==(const move &a, const move &b);
#endif