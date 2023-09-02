#include <iostream>
#include "MinMax.hpp"
#include "MoveGener.hpp"
#include <ctime>
#include <string>
using std::cout;
using std::cin;
using std::endl;
void ptcards(const cards &x) {
    for (int8 i = 0; i < N; i++) {
        for (int8 j = 0; j < x.cardCount[i]; j++) {
            cout << v2s[i] << " ";
        }
    }
}
void ptmove(const move &x) {
    cout << MOVE_TYPES_STR[x.type];
    cout << " ";
    ptcards(x.mainCard);
    if (x.subCard.cardNum() != 0) {
        cout << "- ";
        ptcards(x.subCard);
    }
    cout << endl;
}
cards str2cards(const string &x) {
    const char *p = x.c_str();
    cards result;
    for (int i = 0; p[i] != 0; i++) {
        if (p[i] == 'e' || p[i] == 'E') exit(0); // 输入e或E退出程序
        auto pos = c2v.find(p[i]);
        if (pos == c2v.end()) continue; // 若输入不合法字符直接跳过
        result.cardCount[pos->second]++;
    }
    return result;
}
move getMove(const cards &x, move lastMove, const int8 playerid) {
    possibleMoveSet p(x, lastMove);
    while (true) {
        cout << "---------------------------\n";
        cout << "敌人" << (int)playerid << "的牌：";
        ptcards(x);
        cout << "\n"
             << (playerid == enemy1 ? "我方" : "敌人1") << "上一次出牌：";
        ptmove(lastMove);
        cout << "请输入敌人" << (int)playerid << "出的牌：\n";
        cout << "---------------------------\n";
        string t;
        cin >> t;
        if (t == "e") exit(0);
        cards c = str2cards(t);
        vector<move> plist;
        for (const auto &i : p.moveSet) {
            if (c == i.totalCards()) plist.push_back(i);
        }
        if (plist.size() == 0) {
            cout << "输入错误，请重新输入\n";
            continue;
        } else if (plist.size() == 1) {
            cout << "对方出牌：";
            ptmove(plist[0]);
            return plist[0];
        } else {
            for (int k = 0; (unsigned long long)k < plist.size(); k++) {
                cout << "编号：" << k << "  牌型：";
                ptmove(plist[k]);
            }
            cout << "请输入对方所出的牌的对应编号：";
            int k;
            cin >> k;
            if (k >= 0 && (unsigned long long)k < plist.size())
                return plist[k];
            else {
                cout << "输入错误，请从头重新输入\n";
                continue;
            }
        }
    }
}
int main(int argc, char *argv[]) {
    cout << "---------------------------\n";
    cout << "在任何能输入的时候输入e - 结束本次计算" << endl;
    cout << "x小王 d大王 0代表10 p表示过\n";
    cout << "---------------------------\n";
    string strourcards;
    string strenemycards;
    cards ours;
    cards enemy1card;
    cards enemy2card;
    cout << "请输入我方的牌，输入s进入禁用特定牌型模式:\n";
    cin >> strourcards;
    if (strourcards == "s") {
        while (true) {
            for (moveType i = TYPE_1_SINGLE; i <= TYPE_14_4_2_2; i++) {
                cout << (int)i << MOVE_TYPES_STR[i] << (disables[i] ? "禁用" : "启用") << " ";
            }
            cout << endl;
            cout << "请输入要禁用的牌型编号，操作完毕请输入s：\n";
            string t;
            cin >> t;
            if (t == "s") break;
            if (t == "e") exit(0);
            int p = std::stoi(t);
            if (p >= TYPE_1_SINGLE && p <= TYPE_14_4_2_2) disables[p] = true;
            cout << endl;
        }
        cout << "请输入我方的牌:\n";
        cin >> strourcards;
    }
    ours = str2cards(strourcards);

    cout << "请输入敌人1的牌:\n";
    cin >> strenemycards;
    enemy1card = str2cards(strenemycards);

    cout << "请输入敌人2的牌:\n";
    cin >> strenemycards;
    enemy2card = str2cards(strenemycards);

    move tmp;
    status x;
    x.currentPlayer = our;
    x.enemy1Cards = enemy1card;
    x.enemy2Cards = enemy2card;
    x.ourCards = ours;
    x.lastMove = tmp;
    x.lastMoveOwner = our;
    /*
    if (argc > 1) {
        string t = argv[1];
        if (t == "-enemyfirst") {
            cout << "对手先出：\n";
            cout << "请输入对方出的牌：\n";
            tmp = getMove(x.enemy1Cards, tmp);
            x.enemy1Cards.remove(tmp.mainCard);
            x.enemy1Cards.remove(tmp.subCard);
            x.lastMove = tmp;
        }
    }
    */
    cout << "我方的牌：";
    ptcards(ours);
    cout << endl;
    cout << "敌人1的牌：";
    ptcards(enemy1card);
    cout << endl;
    cout << "敌人2的牌：";
    ptcards(enemy2card);
    cout << endl;

    clock_t start, end;
    int l = 0;
    while (x.ourCards.cardNum() > 0 && x.enemy1Cards.cardNum() > 0 && x.enemy2Cards.cardNum() > 0) {
        cout << "---------------------------\n";
        cout << "开始计算\n";

        cout << "我方的牌：";
        ptcards(x.ourCards);
        cout << endl;
        cout << "敌人1的牌：";
        ptcards(x.enemy1Cards);
        cout << endl;
        cout << "敌人2的牌：";
        ptcards(x.enemy2Cards);
        cout << endl;

        start = clock();
        returned_result result = minMaxSearch(x);
        end = clock();
        if (l == 0) cout << "计算用时：" << (double)(end - start) / CLOCKS_PER_SEC << "秒\n";
        l++;
        if (result.score == minScore) {
            cout << "对方有必胜策略\n";
            exit(0);
        }
        cout << "我方应该出牌：\n";
        ptmove(result.bestMove);
        x.ourCards.remove(result.bestMove.mainCard);
        x.ourCards.remove(result.bestMove.subCard);
        x.lastMove = result.bestMove;
        if (result.bestMove.type != TYPE_0_PASS) x.lastMoveOwner = our;
        if (x.ourCards.cardNum() == 0) {
            cout << "我方胜利\n";
            exit(0);
        }
        // cout << "请输入对方出的牌：\n";
        tmp = getMove(x.enemy1Cards, x.lastMove, enemy1);
        if (tmp.type != TYPE_0_PASS) {
            x.enemy1Cards.remove(tmp.mainCard);
            x.enemy1Cards.remove(tmp.subCard);
            x.lastMove = tmp;
            x.lastMoveOwner = enemy1;
        }
        tmp = getMove(x.enemy2Cards, x.lastMove, enemy2);
        if (tmp.type != TYPE_0_PASS) {
            x.enemy2Cards.remove(tmp.mainCard);
            x.enemy2Cards.remove(tmp.subCard);
            x.lastMove = tmp;
            x.lastMoveOwner = enemy2;
        }
    }
    return 0;
}