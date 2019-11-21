#ifndef OTHELLO_CHESSSITUATION_H
#define OTHELLO_CHESSSITUATION_H


#include "GameLogicEnum.h"
#include "Move.h"
#include "../Entire/ChessCell.h"

//当前棋局局势类
class ChessSituation {
private:


public:

    Enum_ChessCellState chessboard[8][8];                    //棋盘
    Player* nextPlayer;                                      //下一步行动的玩家
    int roundNum;                                           //轮数

    ChessSituation();

};


#endif //OTHELLO_CHESSSITUATION_H
