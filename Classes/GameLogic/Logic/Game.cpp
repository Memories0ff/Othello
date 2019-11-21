#include <iostream>
#include "Game.h"

//游戏开始（玩家之间）
Game *Game::gameStartForHumanToHuman() {
    auto *game = new Game();
//    game->gameState = START;
    game->mode = 2;                                               //游戏模式玩家对战

    auto *blackPlayer = new Player(HUMAN, BLACK);              //执黑玩家
    auto *whitePlayer = new Player(HUMAN, WHITE);              //执白玩家
    game->blackPlayer = blackPlayer;
    game->whitePlayer = whitePlayer;

    game->chessProcessStack = new stack<ChessSituation>();      //保存每一步棋局的栈，用于悔棋功能

    game->roundNum = 1;                                         //开始回合数为1

    game->currentSituation = new ChessSituation();                //设置初始棋局
    game->currentSituation->roundNum = 1;

    //设置初始棋局棋盘
    Enum_ChessCellState originalSituation[8][8] = {
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, WHITE, BLACK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLACK, WHITE, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
    };
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            game->currentSituation->chessboard[i][j] = originalSituation[i][j];
        }
    }

    game->currentSituation->nextPlayer = blackPlayer;        //黑子先落子
    game->chessProcessStack->push(*(game->currentSituation));   //初始棋局压入栈

    return game;
}

//游戏开始（人机之间）
Game *Game::gameStartForHumanToAI(int difficulty, bool humanFirst) {
    auto *game = new Game();
//    game->gameState = START;
    game->difficulty = difficulty;                                //游戏难度
    game->mode = 1;                                               //游戏模式人机

    Player *blackPlayer;
    Player *whitePlayer;
    if (humanFirst) {
        blackPlayer = new Player(HUMAN, BLACK);           //执黑玩家（人类）
        whitePlayer = new Player(AI, WHITE);              //执白玩家（电脑）
        game->aiAction = new AIAction(blackPlayer, whitePlayer);
    } else {
        blackPlayer = new Player(AI, BLACK);              //执黑玩家（电脑）
        whitePlayer = new Player(HUMAN, WHITE);           //执白玩家（人类）
        game->aiAction = new AIAction(whitePlayer, blackPlayer);
    }
    game->blackPlayer = blackPlayer;
    game->whitePlayer = whitePlayer;


    game->chessProcessStack = new stack<ChessSituation>();      //保存每一步棋局的栈，用于悔棋功能

    game->roundNum = 1;                                         //开始回合数为1

    game->currentSituation = new ChessSituation();                //设置初始棋局
    game->currentSituation->roundNum = 1;

    //设置初始棋局棋盘
    Enum_ChessCellState originalSituation[8][8] = {
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, WHITE, BLACK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLACK, WHITE, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
    };
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            game->currentSituation->chessboard[i][j] = originalSituation[i][j];
        }
    }

    game->currentSituation->nextPlayer = blackPlayer;        //黑子先落子
    game->chessProcessStack->push(*(game->currentSituation));   //初始棋局压入栈

    return game;
}

//读取游戏
Game *Game::loadGame(int difficulty, bool humanFirst, int mode, string chessRecord) {
    auto game = new Game();
    game->difficulty = difficulty;
    game->mode = mode;
    //棋谱没有通过playerMove更新
    game->chessRecord = chessRecord;

    //人机模式
    if (mode == 1) {
        Player *blackPlayer;
        Player *whitePlayer;
        if (humanFirst) {
            blackPlayer = new Player(HUMAN, BLACK);           //执黑玩家（人类）
            whitePlayer = new Player(AI, WHITE);              //执白玩家（电脑）
            game->aiAction = new AIAction(blackPlayer, whitePlayer);
        } else {
            blackPlayer = new Player(AI, BLACK);              //执黑玩家（电脑）
            whitePlayer = new Player(HUMAN, WHITE);           //执白玩家（人类）
            game->aiAction = new AIAction(whitePlayer, blackPlayer);
        }
        game->blackPlayer = blackPlayer;
        game->whitePlayer = whitePlayer;
    }//玩家对战
    else {
        auto *blackPlayer = new Player(HUMAN, BLACK);              //执黑玩家
        auto *whitePlayer = new Player(HUMAN, WHITE);              //执白玩家
        game->blackPlayer = blackPlayer;
        game->whitePlayer = whitePlayer;
    }
    game->chessProcessStack = new stack<ChessSituation>();        //棋局栈
    game->roundNum = 1;                                         //开始回合数为1

    game->currentSituation = new ChessSituation();                //设置初始棋局
    game->currentSituation->roundNum = 1;

    //设置初始棋局棋盘
    Enum_ChessCellState originalSituation[8][8] = {
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, WHITE, BLACK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLACK, WHITE, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
        {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
    };
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            game->currentSituation->chessboard[i][j] = originalSituation[i][j];
        }
    }

    game->currentSituation->nextPlayer = game->blackPlayer;        //黑子先落子
    game->chessProcessStack->push(*(game->currentSituation));   //初始棋局压入栈

    //恢复棋局
    while (!chessRecord.empty()) {
        int col = chessRecord[0] - 'A';
        int row = chessRecord[1] - '1';
        //黑方下一步
        if (game->currentSituation->nextPlayer==game->blackPlayer) {
            auto move = new Move(row, col, game->blackPlayer);
            game->playerMove(move, game->currentSituation);
            delete move;
        }//白方下一步
        else {
            auto move = new Move(row, col, game->whitePlayer);
            game->playerMove(move, game->currentSituation);
            delete move;
        }
        chessRecord = chessRecord.substr(2, chessRecord.length() - 2);
    }
    return game;
}

//计算黑子个数
int Game::getCurrentBlackPieceNum() {
    int num = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (this->currentSituation->chessboard[i][j] == BLACK) {
                ++num;
            }
        }
    }
    return num;
}

//计算白子个数
int Game::getCurrentWhitePieceNum() {
    int num = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (this->currentSituation->chessboard[i][j] == WHITE) {
                ++num;
            }
        }
    }
    return num;
}

//判断玩家落子是否有效
bool Game::isPlayerMovingAvailable(Move *move, ChessSituation *currentSituation) {
    int row = move->row;                          //行
    int col = move->col;                          //列
    Player *player = move->player;                //执行操作的玩家
    Enum_ChessCellState mineChesspieceColor = player->chessCellState;                                   //我方棋子颜色
//    Enum_ChessCellState opponentChesspieceColor = (mineChesspieceColor == WHITE ? BLACK : WHITE);     //对方棋子颜色

    int addChesspieceNum = 0;

    if (currentSituation->chessboard[row][col] == BLANK) {

        //上
        for (int distance = 1; row - distance >= 0; ++distance) {
            if (currentSituation->chessboard[row - distance][col] == BLANK) {
                break;
            } else if (currentSituation->chessboard[row - distance][col] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //下
        for (int distance = 1; row + distance <= 7; ++distance) {
            if (currentSituation->chessboard[row + distance][col] == BLANK) {
                break;
            } else if (currentSituation->chessboard[row + distance][col] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //左
        for (int distance = 1; col - distance >= 0; ++distance) {
            if (currentSituation->chessboard[row][col - distance] == BLANK) {
                break;
            } else if (currentSituation->chessboard[row][col - distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //右
        for (int distance = 1; col + distance <= 7; ++distance) {
            if (currentSituation->chessboard[row][col + distance] == BLANK) {
                break;
            } else if (currentSituation->chessboard[row][col + distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //左上
        for (int distance = 1; col - distance >= 0 && row - distance >= 0; ++distance) {
            if (currentSituation->chessboard[row - distance][col - distance] == BLANK) {
                break;
            } else if (currentSituation->chessboard[row - distance][col - distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //右上
        for (int distance = 1; col + distance <= 7 && row - distance >= 0; ++distance) {
            if (currentSituation->chessboard[row - distance][col + distance] == BLANK) {
                break;
            } else if (currentSituation->chessboard[row - distance][col + distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //左下
        for (int distance = 1; col - distance >= 0 && row + distance <= 7; ++distance) {
            if (currentSituation->chessboard[row + distance][col - distance] == BLANK) {
                break;
            } else if (currentSituation->chessboard[row + distance][col - distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //右下
        for (int distance = 1; col + distance <= 7 && row + distance <= 7; ++distance) {
            if (currentSituation->chessboard[row + distance][col + distance] == BLANK) {
                break;
            } else if (currentSituation->chessboard[row + distance][col + distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

    }
    return addChesspieceNum != 0;
}

//判断玩家能否落子（确定下一步轮到哪个玩家，游戏是否结束）
bool Game::isPlayerMovable(Player *player, ChessSituation *currentSituation) {

    Enum_ChessCellState mineChesspieceColor = player->chessCellState;                                   //我方棋子颜色
//    Enum_ChessCellState opponentChesspieceColor = (mineChesspieceColor == WHITE ? BLACK : WHITE);     //对方棋子颜色

    int addChesspieceNum = 0;       //吃子数，不为0则能够落子

    //搜索每一个格子
    for (int row = 0; row < 8; ++row) {
        if (addChesspieceNum != 0) {    //吃子数不为0可以落子，退出循环
            break;
        }
        for (int col = 0; col < 8; ++col) {
            if (addChesspieceNum != 0) {    //吃子数不为0可以落子，退出循环
                break;
            }
            if (currentSituation->chessboard[row][col] == BLANK) {

                //上
                for (int distance = 1; row - distance >= 0; ++distance) {
                    if (currentSituation->chessboard[row - distance][col] == BLANK) {
                        break;
                    } else if (currentSituation->chessboard[row - distance][col] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //下
                for (int distance = 1; row + distance <= 7; ++distance) {
                    if (currentSituation->chessboard[row + distance][col] == BLANK) {
                        break;
                    } else if (currentSituation->chessboard[row + distance][col] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //左
                for (int distance = 1; col - distance >= 0; ++distance) {
                    if (currentSituation->chessboard[row][col - distance] == BLANK) {
                        break;
                    } else if (currentSituation->chessboard[row][col - distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //右
                for (int distance = 1; col + distance <= 7; ++distance) {
                    if (currentSituation->chessboard[row][col + distance] == BLANK) {
                        break;
                    } else if (currentSituation->chessboard[row][col + distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //左上
                for (int distance = 1; col - distance >= 0 && row - distance >= 0; ++distance) {
                    if (currentSituation->chessboard[row - distance][col - distance] == BLANK) {
                        break;
                    } else if (currentSituation->chessboard[row - distance][col - distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //右上
                for (int distance = 1; col + distance <= 7 && row - distance >= 0; ++distance) {
                    if (currentSituation->chessboard[row - distance][col + distance] == BLANK) {
                        break;
                    } else if (currentSituation->chessboard[row - distance][col + distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //左下
                for (int distance = 1; col - distance >= 0 && row + distance <= 7; ++distance) {
                    if (currentSituation->chessboard[row + distance][col - distance] == BLANK) {
                        break;
                    } else if (currentSituation->chessboard[row + distance][col - distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //右下
                for (int distance = 1; col + distance <= 7 && row + distance <= 7; ++distance) {
                    if (currentSituation->chessboard[row + distance][col + distance] == BLANK) {
                        break;
                    } else if (currentSituation->chessboard[row + distance][col + distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

            }
        }
    }

    return addChesspieceNum != 0;
}

//落子，生成新的棋局，先保存再销毁旧的棋局
void Game::playerMove(Move *move, ChessSituation *currentSituation) {

    //复制棋盘
    auto *nextSituation = new ChessSituation();
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            nextSituation->chessboard[i][j] = currentSituation->chessboard[i][j];
        }
    }

    Player *currentPlayer = move->player;        //走这一步的玩家
    int row = move->row;                        //这一步的行
    int col = move->col;                        //这一步的列

    //另一个玩家
    Player *opponentPlayer = (currentPlayer == this->blackPlayer ? (this->whitePlayer) : (this->blackPlayer));

    Enum_ChessCellState minePieceColor = currentPlayer->chessCellState;  //走这一步玩家的棋子颜色
//    Enum_ChessCellState opponentPieceColor = opponentPlayer->chessCellState; //另一个玩家棋子颜色


    auto *changeLocRowOrder = new vector<int>();       //要改变棋子颜色位置的行的序列
    auto *changeLocColOrder = new vector<int>();       //要改变棋子颜色位置的列的序列，与行序列组合就是要改变棋子颜色的位置
    changeLocRowOrder->push_back(row);                  //保存落子位置的行
    changeLocColOrder->push_back(col);                  //保存落子位置的列
    int changePieceNum;                                 //改变棋子颜色的数目

    //寻找改变棋子颜色的位置

    //上
    int upChangePieceNum = 0;             //上方需要改变棋子的数目
    int upEndRow = row;                   //上方需要改变的最远棋子的行位置
    int upEndCol = col;                   //上方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; distance <= row; ++distance) {
        if (nextSituation->chessboard[row - distance][col] == BLANK) {
            break;
        } else if (nextSituation->chessboard[row - distance][col] == minePieceColor) {
            upChangePieceNum = distance - 1;
            upEndRow = upEndRow - upChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (upChangePieceNum > 0) {     //上方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < upChangePieceNum; ++i) {
            changeLocRowOrder->push_back(upEndRow + i);
            changeLocColOrder->push_back(upEndCol);
        }
    }

    //下
    int downChangePieceNum = 0;             //下方需要改变棋子的数目
    int downEndRow = row;                   //下方需要改变的最远棋子的行位置
    int downEndCol = col;                   //下方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; distance <= 7 - row; ++distance) {
        if (nextSituation->chessboard[row + distance][col] == BLANK) {
            break;
        } else if (nextSituation->chessboard[row + distance][col] == minePieceColor) {
            downChangePieceNum = distance - 1;
            downEndRow = downEndRow + downChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (downChangePieceNum > 0) {     //下方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < downChangePieceNum; ++i) {
            changeLocRowOrder->push_back(downEndRow - i);
            changeLocColOrder->push_back(downEndCol);
        }
    }

    //左
    int leftChangePieceNum = 0;             //左方需要改变棋子的数目
    int leftEndRow = row;                   //左方需要改变的最远棋子的行位置
    int leftEndCol = col;                   //左方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; distance <= col; ++distance) {
        if (nextSituation->chessboard[row][col - distance] == BLANK) {
            break;
        } else if (nextSituation->chessboard[row][col - distance] == minePieceColor) {
            leftChangePieceNum = distance - 1;
            leftEndCol = leftEndCol - leftChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (leftChangePieceNum > 0) {     //左方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < leftChangePieceNum; ++i) {
            changeLocRowOrder->push_back(leftEndRow);
            changeLocColOrder->push_back(leftEndCol + i);
        }
    }
    //右
    int rightChangePieceNum = 0;             //右方需要改变棋子的数目
    int rightEndRow = row;                   //右方需要改变的最远棋子的行位置
    int rightEndCol = col;                   //右方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; distance <= 7 - col; ++distance) {
        if (nextSituation->chessboard[row][col + distance] == BLANK) {
            break;
        } else if (nextSituation->chessboard[row][col + distance] == minePieceColor) {
            rightChangePieceNum = distance - 1;
            rightEndCol = rightEndCol + rightChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (rightChangePieceNum > 0) {     //右方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < rightChangePieceNum; ++i) {
            changeLocRowOrder->push_back(rightEndRow);
            changeLocColOrder->push_back(rightEndCol - i);
        }
    }

    //左上
    int leftUpChangePieceNum = 0;             //左上方需要改变棋子的数目
    int leftUpEndRow = row;                   //左上方需要改变的最远棋子的行位置
    int leftUpEndCol = col;                   //左上方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; distance <= col && distance <= row; ++distance) {
        if (nextSituation->chessboard[row - distance][col - distance] == BLANK) {
            break;
        } else if (nextSituation->chessboard[row - distance][col - distance] == minePieceColor) {
            leftUpChangePieceNum = distance - 1;
            leftUpEndCol = leftUpEndCol - leftUpChangePieceNum;
            leftUpEndRow = leftUpEndRow - leftUpChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (leftUpChangePieceNum > 0) {     //左上方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < leftUpChangePieceNum; ++i) {
            changeLocRowOrder->push_back(leftUpEndRow + i);
            changeLocColOrder->push_back(leftUpEndCol + i);
        }
    }

    //右上
    int rightUpChangePieceNum = 0;             //右上方需要改变棋子的数目
    int rightUpEndRow = row;                   //右上方需要改变的最远棋子的行位置
    int rightUpEndCol = col;                   //右上方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; (distance <= 7 - col) && (distance <= row); ++distance) {
        if (nextSituation->chessboard[row - distance][col + distance] == BLANK) {
            break;
        } else if (nextSituation->chessboard[row - distance][col + distance] == minePieceColor) {
            rightUpChangePieceNum = distance - 1;
            rightUpEndCol = rightUpEndCol + rightUpChangePieceNum;
            rightUpEndRow = rightUpEndRow - rightUpChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (rightUpChangePieceNum > 0) {     //右上方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < rightUpChangePieceNum; ++i) {
            changeLocRowOrder->push_back(rightUpEndRow + i);
            changeLocColOrder->push_back(rightUpEndCol - i);
        }
    }

    //左下
    int leftDownChangePieceNum = 0;             //左下方需要改变棋子的数目
    int leftDownEndRow = row;                   //左下方需要改变的最远棋子的行位置
    int leftDownEndCol = col;                   //左下方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; (distance <= col) && (distance <= 7 - row); ++distance) {
        if (nextSituation->chessboard[row + distance][col - distance] == BLANK) {
            break;
        } else if (nextSituation->chessboard[row + distance][col - distance] == minePieceColor) {
            leftDownChangePieceNum = distance - 1;
            leftDownEndCol = leftDownEndCol - leftDownChangePieceNum;
            leftDownEndRow = leftDownEndRow + leftDownChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (leftDownChangePieceNum > 0) {     //左下方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < leftDownChangePieceNum; ++i) {
            changeLocRowOrder->push_back(leftDownEndRow - i);
            changeLocColOrder->push_back(leftDownEndCol + i);
        }
    }

    //右下
    int rightDownChangePieceNum = 0;             //右上方需要改变棋子的数目
    int rightDownEndRow = row;                   //右上方需要改变的最远棋子的行位置
    int rightDownEndCol = col;                   //右上方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; (distance <= 7 - col) && (distance <= 7 - row); ++distance) {
        if (nextSituation->chessboard[row + distance][col + distance] == BLANK) {
            break;
        } else if (nextSituation->chessboard[row + distance][col + distance] == minePieceColor) {
            rightDownChangePieceNum = distance - 1;
            rightDownEndCol = rightDownEndCol + rightDownChangePieceNum;
            rightDownEndRow = rightDownEndRow + rightDownChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (rightDownChangePieceNum > 0) {     //右上方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < rightDownChangePieceNum; ++i) {
            changeLocRowOrder->push_back(rightDownEndRow - i);
            changeLocColOrder->push_back(rightDownEndCol - i);
        }
    }

    //改变棋子颜色的总数
    changePieceNum =
        1 + upChangePieceNum + downChangePieceNum + leftChangePieceNum + rightChangePieceNum +
            leftUpChangePieceNum +
            rightUpChangePieceNum + leftDownChangePieceNum + rightDownChangePieceNum;//1为落子位置的棋子

    //通过保存的改变序列改棋子颜色
    for (int i = 0; i < changePieceNum; ++i) {
        nextSituation->chessboard[(*changeLocRowOrder)[i]][(*changeLocColOrder)[i]] = minePieceColor;
    }

    //下一步轮到哪个玩家
    if (isGameOver(currentSituation)) {
        nextSituation->nextPlayer = NULL;
    } else {
        nextSituation->nextPlayer = (isPlayerMovable(opponentPlayer, nextSituation)) ? opponentPlayer : currentPlayer;
    }
    //回合数加1
    this->roundNum++;

    //棋局对象中的回合数
    nextSituation->roundNum = this->roundNum;

    //更新当前棋局
    this->currentSituation = nextSituation;

    //新棋局入栈
    this->chessProcessStack->push(*(this->currentSituation));

    //销毁当前棋局数据
    delete currentSituation;

    //销毁本函数中使用的用于保存代改变棋子颜色的行列数据
    delete changeLocRowOrder;
    delete changeLocColOrder;

}

//判断游戏是否结束
bool Game::isGameOver(ChessSituation *currentSituation) {
    return !(isPlayerMovable(this->blackPlayer, currentSituation) ||
        isPlayerMovable(this->whitePlayer, currentSituation));
}

//游戏结束
void Game::gameEnd() {
    int blackNum = 0, whiteNum = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            switch (this->currentSituation->chessboard[i][j]) {
                case BLACK:
                    blackNum++;
                    break;
                case WHITE:
                    whiteNum++;
                    break;
                default:
                    break;
            }
        }
    }
    cout << "B:W=" << blackNum << ":" << whiteNum << endl;
    if (blackNum > whiteNum) {
        cout << "Black wins" << endl;
    } else if (blackNum < whiteNum) {
        cout << "White wins" << endl;
    } else {
        cout << "Draw" << endl;
    }
}

//悔棋
void Game::undo() {
    cout << "悔棋前：stack.size()==" << this->chessProcessStack->size() << endl;
    if (this->chessProcessStack->size() <= 1) {
        cout << "已到开局，不可悔棋" << endl;
        return;
    }
    //人机对战
    if (this->mode == 1) {
        //弹出栈条件：栈中不只有开局（>1）且下一个落子的是AI
        do {
            //更改棋局栈
            this->chessProcessStack->pop();
            //更改棋谱记录，删除最后两个字符
            chessRecord = chessRecord.substr(0, chessRecord.length() - 2);
        } while (this->chessProcessStack->size() > 1 && static_cast<ChessSituation>(this->chessProcessStack->top()).nextPlayer != this->aiAction->humanPlayer);
        auto *chessSituation = static_cast<ChessSituation *>(malloc(sizeof(ChessSituation)));
        memcpy(chessSituation, &this->chessProcessStack->top(), sizeof(ChessSituation));
        //释放存放当前棋局的内存
        delete this->currentSituation;
        this->currentSituation = chessSituation;
        //局数恢复
        this->roundNum = this->currentSituation->roundNum;
    }
    //玩家对战
    if (this->mode == 2) {
        //更改棋局栈
        this->chessProcessStack->pop();
        //更改棋谱记录，删除最后两个字符
        chessRecord = chessRecord.substr(0, chessRecord.length() - 2);
        auto *chessSituation = static_cast<ChessSituation *>(malloc(sizeof(ChessSituation)));
        memcpy(chessSituation, &this->chessProcessStack->top(), sizeof(ChessSituation));
        //释放存放当前棋局的内存
        delete this->currentSituation;
        this->currentSituation = chessSituation;
        //局数恢复
        this->roundNum = this->currentSituation->roundNum;
    }
    cout << "悔棋前：stack.size()==" << this->chessProcessStack->size() << endl;
}


Game::~Game() {
    delete this->chessProcessStack;
    delete this->blackPlayer;
    delete this->whitePlayer;
    delete this->currentSituation;
}