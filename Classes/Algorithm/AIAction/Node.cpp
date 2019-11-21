#include "Node.h"

ChessSituationNode::Node::Node() = default;

//构造方法
ChessSituationNode::Node::Node(int roundNum, Player *nextPlayer, Enum_ChessCellState chessboard[8][8]){
    this->roundNum=roundNum;
    this->nextPlayer=nextPlayer;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            this->chessboard[i][j]=chessboard[i][j];
        }
    }
}
