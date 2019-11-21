#ifndef OTHELLO_AIACTION_H
#define OTHELLO_AIACTION_H


#include "../../GameLogic/Entire/ChessCell.h"
#include "../../GameLogic/Logic/Move.h"
#include "Node.h"
#include "MovingDecision.h"
#include <vector>

#define INFINITY 10000000

using namespace std;
using namespace ChessSituationNode;

class AIAction {
private:

    bool isStablePiece(Node *node, int row, int col);                       //是否为稳定子

    inline MovingDecision maxMove(Node *node, int depth, int alpha, int beta);      //max层

    inline MovingDecision minMove(Node *node, int depth, int alpha, int beta);      //min层


public:
//    vector<ChessCell> *stableChesspiece;                          //稳定子存储数组，有稳定子产生就保存在其中（高级难度用）
    int scanNodeNum = 0;                                              //扫描结点数

    Player *aiPlayer;                                               //AI玩家信息
    Player *humanPlayer;                                            //人类玩家信息

//    Move *bestMove;                                                 //最好棋步

    inline bool isLocAvailable(int row, int col, Player *player, Enum_ChessCellState chessboard[8][8]);   //位置能否落子
    inline bool isMovable(Player *player, Node *node);                      //玩家能否落子
//    bool isGameEnd(Node *node);                                     //游戏是否结束
    inline Node *makeMove(Move *move, Node *currentNode);                   //模拟落子（进入下一层搜索）

    inline vector<Move> generateLegalMoves(Node *node);                   //获得所有合法棋步

    inline int getChesspieceNum(Node *node, Player *player);                //计算棋子数
    inline int getActionMobility(Node *node, Player *player);               //计算行动力
    inline int getStablePieceNum(Node *node, Player *player);                //计算稳定子数
    inline int getStablePiecediff(Node *node);                              //计算稳定子数差
    //......
    inline int estimate(Node *node);                                       //估值（简单难度）未完成
    inline int estimateOfFinal(Node *node);                                //终局估值

//    Move* miniMax(Node* node,int depth);                                                        //极小化极大值算法
    MovingDecision getMovingDecision(Node *node, int depth);

    MovingDecision miniMaxWithAlphaBeta(Node *node, int depth, int alpha, int beta);         //极小化极大值算法，使用Alpha-Beta剪枝

    //构造方法
    AIAction(Player *blackPlayer, Player *whitePlayer);

};


#endif //OTHELLO_AIACTION_H
