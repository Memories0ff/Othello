#include <iostream>
#include <ctime>
#include "AIAction.h"

//private

//max层
inline MovingDecision AIAction::maxMove(Node *node, int depth, int alpha, int beta) {
    //到达深度限制，直接返回估值和落子位置
    if (depth == 0) {
        this->scanNodeNum++;            //扫描结点数加1
        //到达深度限制时正好是终局
        if (node->roundNum >= 60) {
            MovingDecision movingDecision = {-1, -1, estimateOfFinal(node)};
            return movingDecision;
        }
            //到达深度限制时没达到终局
        else {
            MovingDecision movingDecision = {-1, -1, estimate(node)};
            return movingDecision;
        }
    }
    //自己无法落子
    if (!isMovable(node->nextPlayer, node)) {
        //对手也无法落子
        if (!isMovable((node->nextPlayer == aiPlayer ? humanPlayer : aiPlayer), node)) {
            this->scanNodeNum++;            //扫描结点数加1
            //没到达深度限制时到达终局，直接返回棋局胜负值和落子位置
            MovingDecision movingDecision = {-1, -1, estimateOfFinal(node)};
            return movingDecision;
        }
        //对手可以继续落子
        node->nextPlayer = (node->nextPlayer == aiPlayer ? humanPlayer : aiPlayer);
        return minMove(node, depth, alpha, beta);
    }

    //自己可以落子

    this->scanNodeNum++;            //扫描结点数加1

    //合法棋步集合
    vector<Move> moves = generateLegalMoves(node);

    //最好分数初值为负无穷
    int bestScore = -INFINITY;

    Move *move = &(moves[0]);
    for (int i = 0; i < moves.size(); ++i) {
        //alpha为当前最好分数和alpha的最大值
        alpha = bestScore > alpha ? bestScore : alpha;
        //alpha大于等于beta，剪枝
        if (alpha >= beta) {
            break;
        }
        //获得在该处落子的估值
        Node *nextLayerNode = makeMove(&(moves[i]), node);
        int value = minMove(nextLayerNode, depth - 1, bestScore > alpha ? bestScore : alpha, beta).value;
        delete nextLayerNode;
        //取大
        if (value > bestScore) {
            bestScore = value;
            move = &(moves[i]);
        }
    }
    MovingDecision movingDecision = {move->row, move->col, bestScore};
    //释放合法棋步集合
//    delete moves;
    return movingDecision;
}

//min层
inline MovingDecision AIAction::minMove(Node *node, int depth, int alpha, int beta) {
    //到达深度限制，直接返回估值和落子位置
    if (depth == 0) {
        this->scanNodeNum++;            //扫描结点数加1
        //到达深度限制时正好是终局
        if (node->roundNum >= 60) {
            MovingDecision movingDecision = {-1, -1, estimateOfFinal(node)};
            return movingDecision;
        }
            //到达深度限制时没达到终局
        else {
            MovingDecision movingDecision = {-1, -1, estimate(node)};
            return movingDecision;
        }
    }
    //自己无法落子
    if (!isMovable(node->nextPlayer, node)) {
        //对手也无法落子
        if (!isMovable((node->nextPlayer == aiPlayer ? humanPlayer : aiPlayer), node)) {
            this->scanNodeNum++;            //扫描结点数加1
            //没到达深度限制时到达终局，直接返回棋局胜负值和落子位置
            MovingDecision movingDecision = {-1, -1, estimateOfFinal(node)};
            return movingDecision;
        }
        //对手可以继续落子
        node->nextPlayer = (node->nextPlayer == aiPlayer ? humanPlayer : aiPlayer);
        return maxMove(node, depth, alpha, beta);
    }

    //自己可以落子

    this->scanNodeNum++;            //扫描结点数加1

    //合法棋步集合
    vector<Move> moves = generateLegalMoves(node);

    //最好分数初值为正无穷
    int bestScore = INFINITY;

    Move *move = &(moves[0]);
    for (int i = 0; i < moves.size(); ++i) {
        //beta为当前最好分数和beta的最小值
        beta = bestScore < beta ? bestScore : beta;
        //alpha大于等于beta，剪枝
        if (alpha >= beta) {
            break;
        }
        //获得在该处落子的估值
        Node *nextLayerNode = makeMove(&(moves[i]), node);
        int value = maxMove(nextLayerNode, depth - 1, alpha, bestScore < beta ? bestScore : beta).value;
        delete nextLayerNode;
        //取小
        if (value < bestScore) {
            bestScore = value;
            move = &(moves[i]);
        }
    }
    MovingDecision movingDecision = {move->row, move->col, bestScore};
    //释放合法棋步集合
//    delete moves;
    return movingDecision;
}

//是否为稳定子
bool AIAction::isStablePiece(Node *node, int row, int col) {
    //排除空格
    if (node->chessboard[row][col] == BLANK) {
        return false;
    }
    //角
    if ((row == 0 || row == 7) && (col == 0 || col == 7)) {
        //必为稳定子
        return true;
    }
        //边
    else if (row == 0 || row == 7 || col == 0 || col == 7) {
        //一行中某一个方向上的棋子都与自己一样为稳定子，一行都有棋子则为稳定子
        if (row == 0 || row == 7) {
            //左到右
            bool leftHalfRowStableFlag = true;                         //非占满一整行即可得知是否为稳定子标记
            bool fullRowStableFlag = true;                         //占满一整行即可得知是否为稳定子标记
            for (int i = 0; i < col; ++i) {
                if (node->chessboard[row][i] == BLANK) {
                    fullRowStableFlag = false;
                    leftHalfRowStableFlag = false;
                } else if (node->chessboard[row][i] != node->chessboard[row][col]) {
                    leftHalfRowStableFlag = false;
                }
            }
            if (leftHalfRowStableFlag) {
                return true;
            }
            //右到左
            bool rightHalfRowStableFlag = true;
            for (int i = 7; i > col; --i) {
                if (node->chessboard[row][i] == BLANK) {
                    fullRowStableFlag = false;
                    rightHalfRowStableFlag = false;
                } else if (node->chessboard[row][i] != node->chessboard[row][col]) {
                    rightHalfRowStableFlag = false;
                }
            }
            if (rightHalfRowStableFlag) {
                return true;
            }
            return fullRowStableFlag;
        }

            //一列中某一个方向上的棋子都与自己一样为稳定子，一列都有棋子则为稳定子
        else if (col == 0 || col == 7) {
            //上到下
            bool upHalfColStableFlag = true;                         //非占满一整列即可得知是否为稳定子标记
            bool fullColStableFlag = true;                         //占满一整列即可得知是否为稳定子标记
            for (int i = 0; i < row; ++i) {
                if (node->chessboard[i][col] == BLANK) {
                    fullColStableFlag = false;
                    upHalfColStableFlag = false;
                } else if (node->chessboard[i][col] != node->chessboard[row][col]) {
                    upHalfColStableFlag = false;
                }
            }
            if (upHalfColStableFlag) {
                return true;
            }
            //下到上
            bool downHalfColStableFlag = true;
            for (int i = 7; i > row; --i) {
                if (node->chessboard[i][col] == BLANK) {
                    fullColStableFlag = false;
                    downHalfColStableFlag = false;
                } else if (node->chessboard[i][col] != node->chessboard[row][col]) {
                    downHalfColStableFlag = false;
                }
            }
            if (downHalfColStableFlag) {
                return true;
            }
            return fullColStableFlag;
        }
    }
        //其他非边非角
    else {
        //水平及垂直方向
        for (int i = 0; i < 8; ++i) {
            if (node->chessboard[i][col] == BLANK || node->chessboard[row][i] == BLANK) {
                return false;
            }
        }
        //左上到右下
        int diff = (row < col) ? (col - row) : (row - col);
        int rowBegin = (row > col) ? diff : 0;
        int colBegin = (row > col) ? 0 : diff;
        for (int i = 0; i < 8 - diff; ++i) {
            if (node->chessboard[rowBegin + i][colBegin + i] == BLANK) {
                return false;
            }
        }
        //左下到右上
        for (int i = 0; i <= row + col; ++i) {
            if (node->chessboard[row + col - i][i] == BLANK) {
                return false;
            }
        }
        //8个方向都有没有空格，为稳定子
        return true;
    }
    return false;
}

//public

//位置能否落子
inline bool AIAction::isLocAvailable(int row, int col, Player *player, Enum_ChessCellState chessboard[8][8]) {
    Enum_ChessCellState mineChesspieceColor = player->chessCellState;                                   //我方棋子颜色

    int addChesspieceNum = 0;

    if (chessboard[row][col] == BLANK) {

        //上
        for (int distance = 1; row - distance >= 0; ++distance) {
            if (chessboard[row - distance][col] == BLANK) {
                break;
            } else if (chessboard[row - distance][col] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //下
        for (int distance = 1; row + distance <= 7; ++distance) {
            if (chessboard[row + distance][col] == BLANK) {
                break;
            } else if (chessboard[row + distance][col] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //左
        for (int distance = 1; col - distance >= 0; ++distance) {
            if (chessboard[row][col - distance] == BLANK) {
                break;
            } else if (chessboard[row][col - distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //右
        for (int distance = 1; col + distance <= 7; ++distance) {
            if (chessboard[row][col + distance] == BLANK) {
                break;
            } else if (chessboard[row][col + distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //左上
        for (int distance = 1; col - distance >= 0 && row - distance >= 0; ++distance) {
            if (chessboard[row - distance][col - distance] == BLANK) {
                break;
            } else if (chessboard[row - distance][col - distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //右上
        for (int distance = 1; col + distance <= 7 && row - distance >= 0; ++distance) {
            if (chessboard[row - distance][col + distance] == BLANK) {
                break;
            } else if (chessboard[row - distance][col + distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //左下
        for (int distance = 1; col - distance >= 0 && row + distance <= 7; ++distance) {
            if (chessboard[row + distance][col - distance] == BLANK) {
                break;
            } else if (chessboard[row + distance][col - distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

        //右下
        for (int distance = 1; col + distance <= 7 && row + distance <= 7; ++distance) {
            if (chessboard[row + distance][col + distance] == BLANK) {
                break;
            } else if (chessboard[row + distance][col + distance] == mineChesspieceColor) {
                addChesspieceNum = addChesspieceNum + (distance - 1);
                break;
            }
        }

    }
    return addChesspieceNum != 0;
}

//玩家能否落子
inline bool AIAction::isMovable(Player *player, Node *node) {

    Enum_ChessCellState mineChesspieceColor = player->chessCellState;                                   //我方棋子颜色

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
            if (node->chessboard[row][col] == BLANK) {

                //上
                for (int distance = 1; row - distance >= 0; ++distance) {
                    if (node->chessboard[row - distance][col] == BLANK) {
                        break;
                    } else if (node->chessboard[row - distance][col] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //下
                for (int distance = 1; row + distance <= 7; ++distance) {
                    if (node->chessboard[row + distance][col] == BLANK) {
                        break;
                    } else if (node->chessboard[row + distance][col] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //左
                for (int distance = 1; col - distance >= 0; ++distance) {
                    if (node->chessboard[row][col - distance] == BLANK) {
                        break;
                    } else if (node->chessboard[row][col - distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //右
                for (int distance = 1; col + distance <= 7; ++distance) {
                    if (node->chessboard[row][col + distance] == BLANK) {
                        break;
                    } else if (node->chessboard[row][col + distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //左上
                for (int distance = 1; col - distance >= 0 && row - distance >= 0; ++distance) {
                    if (node->chessboard[row - distance][col - distance] == BLANK) {
                        break;
                    } else if (node->chessboard[row - distance][col - distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //右上
                for (int distance = 1; col + distance <= 7 && row - distance >= 0; ++distance) {
                    if (node->chessboard[row - distance][col + distance] == BLANK) {
                        break;
                    } else if (node->chessboard[row - distance][col + distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //左下
                for (int distance = 1; col - distance >= 0 && row + distance <= 7; ++distance) {
                    if (node->chessboard[row + distance][col - distance] == BLANK) {
                        break;
                    } else if (node->chessboard[row + distance][col - distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

                //右下
                for (int distance = 1; col + distance <= 7 && row + distance <= 7; ++distance) {
                    if (node->chessboard[row + distance][col + distance] == BLANK) {
                        break;
                    } else if (node->chessboard[row + distance][col + distance] == mineChesspieceColor) {
                        addChesspieceNum = addChesspieceNum + (distance - 1);
                        break;
                    }
                }

            }
        }
    }

    return addChesspieceNum != 0;
}

//模拟落子（进入下一层搜索）
inline Node *AIAction::makeMove(Move *move, Node *currentNode) {

    //复制棋盘
    auto *nextNode = new Node();
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            nextNode->chessboard[i][j] = currentNode->chessboard[i][j];
        }
    }

    Player *currentPlayer = move->player;        //走这一步的玩家
    int row = move->row;                        //这一步的行
    int col = move->col;                        //这一步的列

    //另一个玩家
    Player *opponentPlayer = (currentPlayer == this->aiPlayer ? (this->humanPlayer) : (this->aiPlayer));

    Enum_ChessCellState minePieceColor = currentPlayer->chessCellState;  //走这一步玩家的棋子颜色

    vector<int> changeLocRowOrder;                       //要改变棋子颜色位置的行的序列
    vector<int> changeLocColOrder;                       //要改变棋子颜色位置的列的序列，与行序列组合就是要改变棋子颜色的位置
    changeLocRowOrder.push_back(row);                  //保存落子位置的行
    changeLocColOrder.push_back(col);                  //保存落子位置的列
    int changePieceNum;                                 //改变棋子颜色的数目

    //寻找改变棋子颜色的位置

    //上
    int upChangePieceNum = 0;             //上方需要改变棋子的数目
    int upEndRow = row;                   //上方需要改变的最远棋子的行位置
    int upEndCol = col;                   //上方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; distance <= row; ++distance) {
        if (nextNode->chessboard[row - distance][col] == BLANK) {
            break;
        } else if (nextNode->chessboard[row - distance][col] == minePieceColor) {
            upChangePieceNum = distance - 1;
            upEndRow = upEndRow - upChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (upChangePieceNum > 0) {     //上方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < upChangePieceNum; ++i) {
            changeLocRowOrder.push_back(upEndRow + i);
            changeLocColOrder.push_back(upEndCol);
        }
    }

    //下
    int downChangePieceNum = 0;             //下方需要改变棋子的数目
    int downEndRow = row;                   //下方需要改变的最远棋子的行位置
    int downEndCol = col;                   //下方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; distance <= 7 - row; ++distance) {
        if (nextNode->chessboard[row + distance][col] == BLANK) {
            break;
        } else if (nextNode->chessboard[row + distance][col] == minePieceColor) {
            downChangePieceNum = distance - 1;
            downEndRow = downEndRow + downChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (downChangePieceNum > 0) {     //下方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < downChangePieceNum; ++i) {
            changeLocRowOrder.push_back(downEndRow - i);
            changeLocColOrder.push_back(downEndCol);
        }
    }

    //左
    int leftChangePieceNum = 0;             //左方需要改变棋子的数目
    int leftEndRow = row;                   //左方需要改变的最远棋子的行位置
    int leftEndCol = col;                   //左方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; distance <= col; ++distance) {
        if (nextNode->chessboard[row][col - distance] == BLANK) {
            break;
        } else if (nextNode->chessboard[row][col - distance] == minePieceColor) {
            leftChangePieceNum = distance - 1;
            leftEndCol = leftEndCol - leftChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (leftChangePieceNum > 0) {     //左方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < leftChangePieceNum; ++i) {
            changeLocRowOrder.push_back(leftEndRow);
            changeLocColOrder.push_back(leftEndCol + i);
        }
    }
    //右
    int rightChangePieceNum = 0;             //右方需要改变棋子的数目
    int rightEndRow = row;                   //右方需要改变的最远棋子的行位置
    int rightEndCol = col;                   //右方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; distance <= 7 - col; ++distance) {
        if (nextNode->chessboard[row][col + distance] == BLANK) {
            break;
        } else if (nextNode->chessboard[row][col + distance] == minePieceColor) {
            rightChangePieceNum = distance - 1;
            rightEndCol = rightEndCol + rightChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (rightChangePieceNum > 0) {     //右方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < rightChangePieceNum; ++i) {
            changeLocRowOrder.push_back(rightEndRow);
            changeLocColOrder.push_back(rightEndCol - i);
        }
    }

    //左上
    int leftUpChangePieceNum = 0;             //左上方需要改变棋子的数目
    int leftUpEndRow = row;                   //左上方需要改变的最远棋子的行位置
    int leftUpEndCol = col;                   //左上方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; distance <= col && distance <= row; ++distance) {
        if (nextNode->chessboard[row - distance][col - distance] == BLANK) {
            break;
        } else if (nextNode->chessboard[row - distance][col - distance] == minePieceColor) {
            leftUpChangePieceNum = distance - 1;
            leftUpEndCol = leftUpEndCol - leftUpChangePieceNum;
            leftUpEndRow = leftUpEndRow - leftUpChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (leftUpChangePieceNum > 0) {     //左上方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < leftUpChangePieceNum; ++i) {
            changeLocRowOrder.push_back(leftUpEndRow + i);
            changeLocColOrder.push_back(leftUpEndCol + i);
        }
    }

    //右上
    int rightUpChangePieceNum = 0;             //右上方需要改变棋子的数目
    int rightUpEndRow = row;                   //右上方需要改变的最远棋子的行位置
    int rightUpEndCol = col;                   //右上方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; (distance <= 7 - col) && (distance <= row); ++distance) {
        if (nextNode->chessboard[row - distance][col + distance] == BLANK) {
            break;
        } else if (nextNode->chessboard[row - distance][col + distance] == minePieceColor) {
            rightUpChangePieceNum = distance - 1;
            rightUpEndCol = rightUpEndCol + rightUpChangePieceNum;
            rightUpEndRow = rightUpEndRow - rightUpChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (rightUpChangePieceNum > 0) {     //右上方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < rightUpChangePieceNum; ++i) {
            changeLocRowOrder.push_back(rightUpEndRow + i);
            changeLocColOrder.push_back(rightUpEndCol - i);
        }
    }

    //左下
    int leftDownChangePieceNum = 0;             //左下方需要改变棋子的数目
    int leftDownEndRow = row;                   //左下方需要改变的最远棋子的行位置
    int leftDownEndCol = col;                   //左下方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; (distance <= col) && (distance <= 7 - row); ++distance) {
        if (nextNode->chessboard[row + distance][col - distance] == BLANK) {
            break;
        } else if (nextNode->chessboard[row + distance][col - distance] == minePieceColor) {
            leftDownChangePieceNum = distance - 1;
            leftDownEndCol = leftDownEndCol - leftDownChangePieceNum;
            leftDownEndRow = leftDownEndRow + leftDownChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (leftDownChangePieceNum > 0) {     //左下方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < leftDownChangePieceNum; ++i) {
            changeLocRowOrder.push_back(leftDownEndRow - i);
            changeLocColOrder.push_back(leftDownEndCol + i);
        }
    }

    //右下
    int rightDownChangePieceNum = 0;             //右上方需要改变棋子的数目
    int rightDownEndRow = row;                   //右上方需要改变的最远棋子的行位置
    int rightDownEndCol = col;                   //右上方需要改变的最远棋子的列位置
    //搜寻
    for (int distance = 1; (distance <= 7 - col) && (distance <= 7 - row); ++distance) {
        if (nextNode->chessboard[row + distance][col + distance] == BLANK) {
            break;
        } else if (nextNode->chessboard[row + distance][col + distance] == minePieceColor) {
            rightDownChangePieceNum = distance - 1;
            rightDownEndCol = rightDownEndCol + rightDownChangePieceNum;
            rightDownEndRow = rightDownEndRow + rightDownChangePieceNum;
            break;
        }
    }
    //加入改变序列
    if (rightDownChangePieceNum > 0) {     //右上方需要改变棋子的数目大于0才加入改变序列中
        for (int i = 0; i < rightDownChangePieceNum; ++i) {
            changeLocRowOrder.push_back(rightDownEndRow - i);
            changeLocColOrder.push_back(rightDownEndCol - i);
        }
    }

    //改变棋子颜色的总数
    changePieceNum =
            1 + upChangePieceNum + downChangePieceNum + leftChangePieceNum + rightChangePieceNum +
            leftUpChangePieceNum +
            rightUpChangePieceNum + leftDownChangePieceNum + rightDownChangePieceNum;//1为落子位置的棋子

    //通过保存的改变序列改棋子颜色
    for (int i = 0; i < changePieceNum; ++i) {
        nextNode->chessboard[changeLocRowOrder[i]][changeLocColOrder[i]] = minePieceColor;
    }

    //下一步应该轮到哪个玩家，忽略不能落子和游戏结束的情况
    nextNode->nextPlayer = opponentPlayer;


    //回合数加1
    nextNode->roundNum = currentNode->roundNum + 1;

    return nextNode;
}

//获得所有合法棋步
inline vector<Move> AIAction::generateLegalMoves(Node *node) {
    vector<Move> moves;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (isLocAvailable(i, j, node->nextPlayer, node->chessboard)) {
                Move move(i, j, node->nextPlayer);
                moves.push_back(move);
            }
        }
    }
    return moves;
}

//计算棋子数
inline int AIAction::getChesspieceNum(Node *node, Player *player) {
    int num = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (node->chessboard[i][j] == player->chessCellState) {
                num++;
            }
        }
    }
    return num;
}

//计算行动力
inline int AIAction::getActionMobility(Node *node, Player *player) {
    int moveNum = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (isLocAvailable(i, j, player, node->chessboard)) {
                moveNum++;
            }
        }
    }
    return moveNum;
}

//计算稳定子数
inline int AIAction::getStablePieceNum(Node *node, Player *player) {
    int num = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((node->chessboard[i][j] == player->chessCellState) && (isStablePiece(node, i, j))) {
                num++;
            }
        }
    }
    return num;
}

//计算稳定子数差
inline int AIAction::getStablePiecediff(Node *node) {
    int aiStablePieceNum = 0;
    int humanStablePieceNum = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((node->chessboard[i][j] == aiPlayer->chessCellState) && (isStablePiece(node, i, j))) {
                aiStablePieceNum++;
            } else if ((node->chessboard[i][j] == humanPlayer->chessCellState) && (isStablePiece(node, i, j))) {
                humanStablePieceNum++;
            }
        }
    }
    return aiStablePieceNum - humanStablePieceNum;
}

//估值
inline int AIAction::estimate(Node *node) {
    return getActionMobility(node, aiPlayer) - getActionMobility(node, humanPlayer) + (getStablePiecediff(node) * 100);
}

//终局估值
inline int AIAction::estimateOfFinal(Node *node) {
    //尽可能保证胜利
//    int aiNum = getChesspieceNum(node, aiPlayer);
//    int humanNum = getChesspieceNum(node, humanPlayer);
//    if (aiNum > humanNum) {
//        return INFINITY;
//    } else if (aiNum < humanNum) {
//        return -INFINITY;
//    } else {
//        return 0;
//    }
    //尽可能多占子
    return getChesspieceNum(node, aiPlayer) - getChesspieceNum(node, humanPlayer);
}

MovingDecision AIAction::getMovingDecision(Node *node, int depth) {
    MovingDecision movingDecision = miniMaxWithAlphaBeta(node, depth, -INFINITY, INFINITY);
    cout<<"回合数："<<node->roundNum<<endl;
    cout<<"搜索层数："<<depth<<endl;
    return movingDecision;
}

//极小化极大值算法，使用Alpha-Beta剪枝
MovingDecision AIAction::miniMaxWithAlphaBeta(Node *node, int depth, int alpha, int beta) {
    //到达深度限制，直接返回估值和落子位置
    if (depth == 0) {
        this->scanNodeNum++;            //扫描结点数加1
        //到达深度限制时正好是终局
        if (node->roundNum >= 60) {
            MovingDecision movingDecision = {-1, -1, estimateOfFinal(node)};
            return movingDecision;
        }
            //到达深度限制时没达到终局
        else {
            MovingDecision movingDecision = {-1, -1, estimate(node)};
            return movingDecision;
        }
    }
    //自己无法落子
    if (!isMovable(node->nextPlayer, node)) {
        //对手也无法落子
        if (!isMovable((node->nextPlayer == aiPlayer ? humanPlayer : aiPlayer), node)) {
            this->scanNodeNum++;            //扫描结点数加1
            //没到达深度限制时到达终局，直接返回棋局胜负值和落子位置
            MovingDecision movingDecision = {-1, -1, estimateOfFinal(node)};
            return movingDecision;
        }
        //对手可以继续落子
        node->nextPlayer = (node->nextPlayer == aiPlayer ? humanPlayer : aiPlayer);
        return minMove(node, depth, alpha, beta);
    }

    //自己可以落子

    this->scanNodeNum++;            //扫描结点数加1

    //合法棋步集合
    vector<Move> moves = generateLegalMoves(node);

    //最好分数初值为负无穷
    int bestScore = -INFINITY;

    Move *move = &(moves[0]);
    if (node->nextPlayer == aiPlayer) {
        for (int i = 0; i < moves.size(); ++i) {
            //alpha为当前最好分数和alpha的最大值
            alpha = bestScore > alpha ? bestScore : alpha;
            //alpha大于等于beta，剪枝
            if (alpha >= beta) {
                break;
            }
            //获得在该处落子的估值
            Node *nextLayerNode = makeMove(&(moves[i]), node);
            int value = minMove(nextLayerNode, depth - 1, bestScore > alpha ? bestScore : alpha, beta).value;
            delete nextLayerNode;
            //取大
            if (value > bestScore) {
                bestScore = value;
                move = &(moves[i]);
            }
        }
        MovingDecision movingDecision = {move->row, move->col, bestScore};
        return movingDecision;
    } else {
        for (int i = 0; i < moves.size(); ++i) {
            //beta为当前最好分数和beta的最小值
            beta = bestScore < beta ? bestScore : beta;
            //alpha大于等于beta，剪枝
            if (alpha >= beta) {
                break;
            }
            //获得在该处落子的估值
            Node *nextLayerNode = makeMove(&(moves[i]), node);
            int value = maxMove(nextLayerNode, depth - 1, alpha, bestScore < beta ? bestScore : beta).value;
            delete nextLayerNode;
            //取小
            if (value < bestScore) {
                bestScore = value;
                move = &(moves[i]);
            }
        }
        MovingDecision movingDecision = {move->row, move->col, bestScore};
        return movingDecision;
    }
}

//构造方法
AIAction::AIAction(Player *humanPlayer, Player *aiPlayer) {
    this->humanPlayer = humanPlayer;
    this->aiPlayer = aiPlayer;
}
