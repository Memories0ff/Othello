#ifndef OTHELLO_MOVE_H
#define OTHELLO_MOVE_H


#include "GameLogicEnum.h"
#include "../Entire/ChessCell.h"
#include "../Entire/Player.h"

//玩家行动（落子）类,表示玩家即将做出的行动
class Move {
public:
    int row;                                                            //行
    int col;                                                            //列
    Player *player;                                                     //执行本行动的玩家
    Move(int row, int col, Player *player);                            //由即将落子的位置和玩家生成真正的行动对象
};


#endif //OTHELLO_MOVE_H
