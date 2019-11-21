#include "Player.h"

//玩家对象构造方法（棋子个数默认为开局的2个，游戏开始后不会去构建新的玩家对象）
Player::Player(Enum_PlayerType playerType,Enum_ChessCellState chessCellState){
    this->playerType=playerType;
    this->chessCellState=chessCellState;
}