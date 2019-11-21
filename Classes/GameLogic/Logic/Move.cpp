#include "Move.h"

//由即将落子的格子和玩家生成真正的行动对象
Move::Move(int row, int col, Player *player) {
    this->col = col;
    this->row = row;
    this->player = player;
}