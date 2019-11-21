#ifndef OTHELLO_PLAYER_H
#define OTHELLO_PLAYER_H

#include "../Logic/GameLogicEnum.h"

class Player {
public:
    Enum_PlayerType playerType;                                                                      //玩家种类（人类或AI）
    Enum_ChessCellState chessCellState;                                                              //玩家所执的棋子（黑或白）

    Player(Enum_PlayerType playerType,Enum_ChessCellState chessCellState);            //玩家对象构造方法（棋子个数默认为开局的2个，游戏开始后不会去构建新的玩家对象）
};


#endif //OTHELLO_PLAYER_H
