#ifndef OTHELLO_GAME_H
#define OTHELLO_GAME_H


#include <vector>
#include <stack>
#include <string>
#include "../Entire/ChessCell.h"
#include "ChessSituation.h"
#include "Move.h"
#include "GameLogicEnum.h"
#include "../Entire/Player.h"
#include "AIAction.h"

using namespace std;

//黑白棋游戏类
class Game {
private:
//    Enum_GameState gameState;                                    //游戏状态

public:
    stack<ChessSituation> *chessProcessStack;                    //保存每一步棋局的栈，用于悔棋功能
    Player *blackPlayer;                                         //执黑玩家
    Player *whitePlayer;                                         //执白玩家
    int roundNum;                                                //回合数
    ChessSituation *currentSituation;                            //当前棋局
    int difficulty;                                              //难度1->3对应简单中等困难
    int mode;                                                    //游戏模式，1人机，2玩家对战
    AIAction *aiAction;                                          //游戏AI
    string chessRecord;                                          //棋谱
    
    static Game *gameStartForHumanToHuman();                                     //游戏开始（玩家之间）
    static Game *gameStartForHumanToAI(int difficulty,bool humanFirst);          //游戏开始（人机之间）
    static Game *loadGame(int difficulty,bool humanFirst,int mode,string chessRecord); //读取游戏                                                    //读取游戏

//    void paintChessboard();                                     //绘制棋盘
    void gameEnd();                                             //游戏结束

    int getCurrentBlackPieceNum();                              //计算黑子个数
    int getCurrentWhitePieceNum();                              //计算白字个数

    //判断玩家落子是否有效
    bool isPlayerMovingAvailable(Move *move, ChessSituation *currentSituation);

    //判断玩家能否落子（确定下一步轮到哪个玩家，游戏是否结束）
    bool isPlayerMovable(Player *player, ChessSituation *currentSituation);

    //落子，返回新的棋局，先保存再销毁旧的棋局
    void playerMove(Move *move, ChessSituation *currentSituation);

    //判断游戏是否结束
    bool isGameOver(ChessSituation *currentSituation);

    //悔棋
    void undo();

    ~Game();                                                    //析构函数
};


#endif //OTHELLO_GAME_H
