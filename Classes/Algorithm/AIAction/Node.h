#ifndef OTHELLO_NODE_H
#define OTHELLO_NODE_H


#include "../../GameLogic/Logic/GameLogicEnum.h"
#include "../../GameLogic/Logic/Move.h"
#include <vector>

using namespace std;

namespace ChessSituationNode{
    class Node {
    public:
        int roundNum;                                           //回合数
        Player *nextPlayer;                                     //轮到落子的玩家
        Enum_ChessCellState chessboard[8][8];                    //棋盘
    //    int value;                                              //结点值

        Node();                                                                             //构造方法
        Node(int roundNum, Player *nextPlayer, Enum_ChessCellState chessboard[8][8]);     //构造方法
    };
}

#endif //OTHELLO_NODE_H
