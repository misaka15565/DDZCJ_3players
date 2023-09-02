#ifndef MOVEGENER_HPP
#define MOVEGENER_HPP

#include <set>
#include <vector>
#include <algorithm>
#include "DataStructure.hpp"
using std::set;
using std::vector;
using std::sort;
extern std::array<bool, 15> disables;
class possibleMoveSet {
private:
    set<cardVal> singles;
    set<cardVal> pairs;
    set<cardVal> triples;
    set<cardVal> bombs;
    void findSingles(const cards &x);
    void findPairs(const cards &x);
    void findTriples(const cards &x);
    void findBombs(const cards &x);
    void findSerial(const set<cardVal> &x, const moveType type,
                    const int minLength);
    void genSerial(const cardVal begin, const cardVal end,
                   const moveType type, const int minLength);
    vector<cards> findSerialTriples() const;
    vector<cards> genSerialTriples(const cardVal begin,
                                   const cardVal end) const;
    vector<cards> genSubcards(
        const cards &x, int8 n) const;

    void genTYPE_0_PASS(const cards &x);
    void genTYPE_1_SINGLE(const cards &x);
    void genTYPE_2_PAIR(const cards &x);
    void genTYPE_3_TRIPLE(const cards &x);
    void genTYPE_4_BOMB(const cards &x);
    void genTYPE_5_KING_BOMB(const cards &x);
    void genTYPE_6_3_1(const cards &x);
    void genTYPE_7_3_2(const cards &x);
    void genTYPE_8_SERIAL_SINGLE(const cards &x);
    void genTYPE_9_SERIAL_PAIR(const cards &x);
    void genTYPE_10_SERIAL_TRIPLE(const cards &x);
    void genTYPE_11_SERIAL_3_1(const cards &x);
    void genTYPE_12_SERIAL_3_2(const cards &x);
    void genTYPE_13_4_2(const cards &x);
    void genTYPE_14_4_2_2(const cards &x);

public:
    vector<move> moveSet;
    possibleMoveSet(const cards &x, move lastMove);
};

#endif