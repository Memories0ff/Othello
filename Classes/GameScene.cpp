#include "GameScene.h"

#define GRIDSIZE 60
#define PIECESIZE 60

Scene *GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }
    isGameSceneEnabled=true;
    //棋盘点击响应打开
    isChessBoardEnabled = true;
    //创建选项图层
    createOptionsLayer();
    //创建棋盘图层
    createChessBoardLayer();
    //创建信息显示图层
    createInfoLayer();
    return true;
}

void GameScene::update(float dt) {
    //更新界面
    updateInterface();
}

//选项图层
void GameScene::createOptionsLayer() {

    auto visibleSize = Director::getInstance()->getVisibleSize();

    //菜单图层
    optionsLayer = Layer::create();
    optionsLayer->setTag(1);
    this->addChild(optionsLayer);

    //返回按钮
    returnMainButton = Button::create();
    returnMainButton->setTitleText("返回主菜单");
    returnMainButton->setTitleFontSize(19);
    returnMainButton->setPosition(Vec2(visibleSize.width / 5, visibleSize.height / 11));
    returnMainButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Return main options" << endl;
                isGameSceneEnabled=false;
                //返回到主界面
                Director::getInstance()->popScene();
                break;
            default:
                break;
        }
    });
    optionsLayer->addChild(returnMainButton);

    //悔棋按钮
    undoButton = Button::create();
    undoButton->setTitleText("悔棋");
    undoButton->setTitleFontSize(19);
    undoButton->setPosition(Vec2(visibleSize.width / 5 * 2, visibleSize.height / 11));
    undoButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Undo game" << endl;
                //悔棋
                game->undo();
                //保存棋谱按钮失效
                saveChessRecordButton->setTitleColor(Color3B(150, 150, 150));
                saveChessRecordButton->setEnabled(false);
                //保存游戏按钮激活
                saveGameButton->setTitleColor(Color3B(255, 255, 255));
                saveGameButton->setEnabled(true);
                //改回保存棋谱按钮
                saveChessRecordButton->setEnabled(false);
                saveChessRecordButton->setTitleColor(Color3B(150, 150, 150));
                saveChessRecordButton->setTitleText("保存棋谱");
                //玩家开局不可悔棋不可保存游戏
                if (game->roundNum <= 1) {
                    //悔棋按钮失效
                    undoButton->setTitleColor(Color3B(150, 150, 150));
                    undoButton->setEnabled(false);
                    //保存按钮失效
                    saveGameButton->setTitleColor(Color3B(150, 150, 150));
                    saveGameButton->setEnabled(false);
                }
                if (game->mode == 1 && game->blackPlayer == game->aiAction->aiPlayer && game->roundNum <= 2) {
                    //悔棋按钮失效
                    undoButton->setTitleColor(Color3B(150, 150, 150));
                    undoButton->setEnabled(false);
                    //保存按钮失效
                    saveGameButton->setTitleColor(Color3B(150, 150, 150));
                    saveGameButton->setEnabled(false);
                }
                if (game->currentSituation->nextPlayer == game->blackPlayer) {
                    infoLabel->setString("轮到黑子");
                } else {
                    infoLabel->setString("轮到白子");
                }
//                chessRecord=game->chessRecord;
                break;
            default:
                break;
        }
    });
    //悔棋按钮失效
    undoButton->setTitleColor(Color3B(150, 150, 150));
    undoButton->setEnabled(false);
    optionsLayer->addChild(undoButton);

    //保存游戏按钮
    saveGameButton = Button::create();
    saveGameButton->setTitleText("保存游戏");
    saveGameButton->setTitleFontSize(19);
    saveGameButton->setPosition(Vec2(visibleSize.width / 5 * 3, visibleSize.height / 11));
    saveGameButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Save game" << endl;
                saveGame();
                //保存游戏
                break;
            default:
                break;
        }
    });
    //保存游戏按钮失效
    saveGameButton->setTitleColor(Color3B(150, 150, 150));
    saveGameButton->setEnabled(false);
    optionsLayer->addChild(saveGameButton);

    //保存棋谱按钮
    saveChessRecordButton = Button::create();
    saveChessRecordButton->setTitleText("保存棋谱");
    saveChessRecordButton->setTitleFontSize(19);
    saveChessRecordButton->setPosition(Vec2(visibleSize.width / 5 * 4, visibleSize.height / 11));
    saveChessRecordButton->setEnabled(false);
    saveChessRecordButton->setTitleColor(Color3B(150, 150, 150));
    saveChessRecordButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Save game record" << endl;
                //保存棋谱
                cout<<"棋谱："<<game->chessRecord<<endl;
                saveChessRecord();
                //失效保存棋谱按钮
                saveChessRecordButton->setEnabled(false);
                saveChessRecordButton->setTitleColor(Color3B(150, 150, 150));
                saveChessRecordButton->setTitleText("棋谱已保存");
                break;
            default:
                break;
        }
    });
    optionsLayer->addChild(saveChessRecordButton);

    //玩家白棋后手需要开始按钮
    //开始按钮
    startButton = Button::create();
    startButton->setTitleText("开始");
    startButton->setTitleFontSize(19);
    startButton->setPosition(Vec2(visibleSize.width / 5 * 4, 2 * visibleSize.height / 11));
    startButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Start Game" << endl;
                //此时棋盘点击响应关闭，按下开始按钮后打开
                isChessBoardEnabled = true;
                //AI黑方落子
                blackAIDo();
                //开始按钮失效
                startButton->setTitleText("游戏已经开始");
                startButton->setTitleColor(Color3B(150, 150, 150));
                startButton->setEnabled(false);
                //悔棋按钮失效
                undoButton->setEnabled(false);
                undoButton->setTitleColor(Color3B(150, 150, 150));
                //保存按钮失效
                saveGameButton->setTitleColor(Color3B(150, 150, 150));
                saveGameButton->setEnabled(false);
                break;
            default:
                break;
        }
    });
    optionsLayer->addChild(startButton);

}

//游戏信息图层
void GameScene::createInfoLayer() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    //菜单图层
    infoLayer = Layer::create();
    infoLayer->setTag(3);
    this->addChild(infoLayer);

    //玩家标签
    playerLabel = Label::create();
    playerLabel->setString("黑:白");
    playerLabel->setSystemFontSize(19);
    playerLabel->setPosition(visibleSize.width / 4 * 3, visibleSize.height / 4 * 3);
    infoLayer->addChild(playerLabel);

    //信息标签
    infoLabel = Label::create();
    infoLabel->setString("黑子先");
    infoLabel->setSystemFontSize(19);
    infoLabel->setPosition(visibleSize.width / 4 * 3, visibleSize.height / 4 * 3 - 70);
    infoLayer->addChild(infoLabel);

    //分数标签
    scoreLabel = Label::create();
    scoreLabel->setString("2:2");
    scoreLabel->setSystemFontSize(19);
    scoreLabel->setPosition(visibleSize.width / 4 * 3, visibleSize.height / 4 * 3 - 30);
    infoLayer->addChild(scoreLabel);
}

//棋盘图层
void GameScene::createChessBoardLayer() {

    auto visibleSize = Director::getInstance()->getVisibleSize();

    chessBoardLayer = Layer::create();
    chessBoardLayer->setTag(2);
    this->addChild(chessBoardLayer);

    chessBoardSprite = Sprite::create("chess_board.png");
    chessBoardSprite->setScale(visibleSize.height * 3 / 4 / chessBoardSprite->getTextureRect().getMaxY());
    chessBoardSprite->setPosition(visibleSize.width / 20 + chessBoardSprite->getTextureRect().getMaxX() / 4, visibleSize.height / 5.6 + chessBoardSprite->getTextureRect().getMaxY() / 4);
    chessBoardLayer->addChild(chessBoardSprite);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onChessBoardTouchBegan, this);
//    listener->onTouchEnded=CC_CALLBACK_2(GameScene::onChessBoardTouchEnded,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, chessBoardSprite);
}

//棋盘点击按钮事件
bool GameScene::onChessBoardTouchBegan(Touch *touch, Event *event) {
    if (isChessBoardEnabled && (!(game->isGameOver(game->currentSituation)))) {
        //点击棋盘后要暂时关闭棋盘点击响应以用于处理落子操作
        isChessBoardEnabled = false;
        //棋格大小
        int gridSize = GRIDSIZE;
        //获取精灵
        auto target = static_cast<Sprite *>(event->getCurrentTarget());
        //获取以棋盘精灵左下角为坐标原点的点击位置
        Point locationInChessBoard = target->convertToNodeSpace(touch->getLocation());
        //获取精灵尺寸
        Size chessBoardSize = target->getContentSize();
        //获取精灵矩形框，以左下角为原点
        Rect rect = Rect(0, 0, chessBoardSize.width, chessBoardSize.height);
        //判断点击位置是否在这个矩形框上
        if (rect.containsPoint(locationInChessBoard)) {
            int row = 7 - (int) locationInChessBoard.y / gridSize;
            int col = (int) locationInChessBoard.x / gridSize;
            cout << "点击棋盘:(" << row << "," << col << ")" << endl;

            //当前游戏为人机模式
            if (game->mode == 1) {
                //AI白方
                if (game->whitePlayer == aiAction->aiPlayer) {
                    //游戏未结束
                    if (!(game->isGameOver(game->currentSituation))) {
                        bool moveSuccess = false;
                        //黑方能落子
                        if (game->currentSituation->nextPlayer == game->blackPlayer) {
                            moveSuccess = blackHumanDo(row, col);
                            if (moveSuccess) {
                                humanContinueFlag = true;
                            }
                        }
                        //白方能落子，新建线程用于思考落子，并解决人机不能分开显示落子问题
//                        whiteAIDo();
                        if (moveSuccess) {
                            thread tWhiteAIDo(&GameScene::whiteAIDo, this);
                            tWhiteAIDo.detach();
                        } else {
                            //打开棋盘点击响应
                            isChessBoardEnabled = true;
                        }
                    }
                    //游戏结束
                    if (game->isGameOver(game->currentSituation)) {
                        //保存棋谱按钮激活
                        saveChessRecordButton->setTitleColor(Color3B(255, 255, 255));
                        saveChessRecordButton->setEnabled(true);
                        //保存游戏按钮失效
                        saveGameButton->setTitleColor(Color3B(150, 150, 150));
                        saveGameButton->setEnabled(false);
                        cout << "游戏结束" << endl;
//                        cout << game->chessRecord << endl;
                        //Game对象执行结束游戏操作
                        game->gameEnd();
                    }
                }
                    //AI黑方
                else if (game->blackPlayer == aiAction->aiPlayer) {
                    //游戏未结束
                    if (!(game->isGameOver(game->currentSituation))) {
                        bool moveSuccess = false;
                        //白方能落子
                        if (game->currentSituation->nextPlayer == game->whitePlayer) {
                            moveSuccess = whiteHumanDo(row, col);
                            if (moveSuccess) {
                                humanContinueFlag = true;
                            }
                        }
                        //黑方能落子，新建线程用于思考落子，并解决人机不能分开显示落子问题
//                        blackAIDo();
                        if (moveSuccess) {
                            thread tBlackAIDo(&GameScene::blackAIDo, this);
                            tBlackAIDo.detach();
                        } else {
                            //打开棋盘点击响应
                            isChessBoardEnabled = true;
                        }
                    }
                    //游戏结束
                    if (game->isGameOver(game->currentSituation)) {
                        //保存棋谱按钮激活
                        saveChessRecordButton->setTitleColor(Color3B(255, 255, 255));
                        saveChessRecordButton->setEnabled(true);
                        //保存游戏按钮失效
                        saveGameButton->setTitleColor(Color3B(150, 150, 150));
                        saveGameButton->setEnabled(false);
                        cout << "游戏结束" << endl;
//                        cout << game->chessRecord << endl;
                        //Game对象执行结束游戏操作
                        game->gameEnd();
                    }
                }
            }
                //当前游戏为玩家对战模式
            else if (game->mode == 2) {
                //游戏未结束
                if (!(game->isGameOver(game->currentSituation))) {
                    //黑方能落子
                    if (game->currentSituation->nextPlayer == game->blackPlayer) {
                        if (!blackHumanDo(row, col)) {
                            isChessBoardEnabled = true;
                            return true;
                        }
                        //激活悔棋按钮
                        undoButton->setEnabled(true);
                        undoButton->setTitleColor(Color3B(255, 255, 255));
                        //激活保存游戏按钮
                        saveGameButton->setEnabled(true);
                        saveGameButton->setTitleColor(Color3B(255, 255, 255));
                        //下一步白方走
                        if (game->currentSituation->nextPlayer == game->whitePlayer) {
                            //信息标签显示为轮到白子
                            infoLabel->setString("轮到白子");
                        } else {
                            //信息标签显示为轮到黑子
                            infoLabel->setString("黑子继续");
                        }
                    }//白方能落子
                    else {
                        if (!whiteHumanDo(row, col)) {
                            isChessBoardEnabled = true;
                            return true;
                        }
                        //激活悔棋按钮
                        undoButton->setEnabled(true);
                        undoButton->setTitleColor(Color3B(255, 255, 255));
                        //激活保存游戏按钮
                        saveGameButton->setEnabled(true);
                        saveGameButton->setTitleColor(Color3B(255, 255, 255));
                        //下一步黑方走
                        if (game->currentSituation->nextPlayer == game->blackPlayer) {
                            //信息标签显示为轮到黑子
                            infoLabel->setString("轮到黑子");
                        } else {
                            //信息标签显示为轮到白子
                            infoLabel->setString("白子继续");
                        }
                    }
                }
                //游戏结束
                if (game->isGameOver(game->currentSituation)) {
                    //保存棋谱按钮激活
                    saveChessRecordButton->setTitleColor(Color3B(255, 255, 255));
                    saveChessRecordButton->setEnabled(true);
                    //保存游戏按钮失效
                    saveGameButton->setTitleColor(Color3B(150, 150, 150));
                    saveGameButton->setEnabled(false);
                    //信息标签显示为游戏结束
                    if(game->getCurrentBlackPieceNum()>game->getCurrentWhitePieceNum()){
                        infoLabel->setString("游戏结束\n  黑方胜");
                    }else if(game->getCurrentBlackPieceNum()<game->getCurrentWhitePieceNum()){
                        infoLabel->setString("游戏结束\n  白方胜");
                    }else{
                        infoLabel->setString("游戏结束\n    平局");
                    }
                    cout << "游戏结束" << endl;
//                    cout << game->chessRecord << endl;
                    //Game对象执行结束游戏操作
                    game->gameEnd();
                }
            }
        } else {
            //点击到无效区域时会关闭棋盘点击响应，之后要打开
            isChessBoardEnabled = true;
        }
        //此时只能在玩家对战模式下打开棋盘点击响应，否则人机对战子线程在思考时能够接受棋盘点击
        if (game->mode == 2) {
            isChessBoardEnabled = true;
        }
    } else {
        cout << "点击无效" << endl;
    }
    return true;
}

//棋盘绘制
void GameScene::paintChessBoard() {
    //移除chessBoardLayer下的所有棋子结点
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessPieceSprite[i][j] != NULL) {
                chessBoardLayer->removeChild(chessPieceSprite[i][j], true);
            }
        }
    }
    //设置棋子精灵结点
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            switch (game->currentSituation->chessboard[i][j]) {
                case BLACK:
                    chessPieceSprite[i][j] = Sprite::create("black_piece_0.png");
                    break;
                case WHITE:
                    chessPieceSprite[i][j] = Sprite::create("white_piece_0.png");
                    break;
                default:
                    chessPieceSprite[i][j] = NULL;
                    break;
            }
        }
    }
    //再将棋子结点重新添加为chessBoardLayer的子结点
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessPieceSprite[i][j] != NULL) {
                chessPieceSprite[i][j]->setScale(PIECESIZE / chessPieceSprite[i][j]->getTextureRect().getMaxY() / 2.2);
                Point locationInWorld = chessBoardSprite->convertToWorldSpace(Vec2(PIECESIZE / 2 + j * PIECESIZE, PIECESIZE / 2 + (7 - i) * PIECESIZE));
                chessPieceSprite[i][j]->setPosition(locationInWorld);
                chessBoardLayer->addChild(chessPieceSprite[i][j]);
            }
        }
    }
}

//更新信息显示
void GameScene::updateInfoLayer() {
    //棋子数信息显示更新
    int numOfBlack = 0;
    int numOfWhite = 0;
    numOfBlack = game->getCurrentBlackPieceNum();
    numOfWhite = game->getCurrentWhitePieceNum();
    ostringstream oss;
    oss << numOfBlack << ":" << numOfWhite << endl;
    scoreLabel->setString(oss.str());
}

//更新界面显示
void GameScene::updateInterface() {
    //棋盘显示更新
    paintChessBoard();
    //信息显示更新
    updateInfoLayer();
}

//黑方玩家操作
bool GameScene::blackHumanDo(int row, int col) {
    //黑方落子信息
    Move *blackMove = new Move(row, col, game->blackPlayer);
    //黑方落子有效
    if (game->isPlayerMovingAvailable(blackMove, game->currentSituation)) {
        //黑方执行落子
        game->playerMove(blackMove, game->currentSituation);
        //重绘棋盘
//        updateInterface();
        delete blackMove;
//        usleep(10000000);
        //棋谱信息更新
        ostringstream oss;
        oss << colNumCharacterTable[col] << row + 1;
        game->chessRecord += oss.str();
        return true;
    }
        //黑方落子无效
    else {
        cout << "黑方落子无效" << endl;
        return false;
    }
}

//白方玩家操作
bool GameScene::whiteHumanDo(int row, int col) {
    //白方落子信息
    Move *whiteMove = new Move(row, col, game->whitePlayer);
    //白方落子有效
    if (game->isPlayerMovingAvailable(whiteMove, game->currentSituation)) {
        //白方执行落子
        game->playerMove(whiteMove, game->currentSituation);
        //重绘棋盘
//        updateInterface();
        delete whiteMove;
//        usleep(10000000);
        //棋谱信息更新
        ostringstream oss;
        oss << colNumCharacterTable[col] << row + 1;
        game->chessRecord += oss.str();
        return true;
    }
        //白方落子无效
    else {
        cout << "白方落子无效" << endl;
        return false;
    }
}

//黑方AI操作
bool GameScene::blackAIDo() {
    //关闭棋盘点击响应
    isChessBoardEnabled = false;
    //某些按钮设置为不可使用
    undoButton->setEnabled(false);
    undoButton->setTitleColor(Color3B(150, 150, 150));
    saveGameButton->setEnabled(false);
    saveGameButton->setTitleColor(Color3B(150, 150, 150));
    //信息标签显示为思考中
    infoLabel->setString("思考中...");
    //等待直到棋盘点击响应执行结束，等待0.3s，给玩家反应时间
//    usleep(300000);
    //在游戏没有结束且下一盘仍然是AI方的情况下继续落子
    while ((!(game->isGameOver(game->currentSituation))) && (game->currentSituation->nextPlayer == game->blackPlayer)) {
        if (aiContinueFlag) {
            infoLabel->setString("黑子继续\n思考中...");
        }
        aiContinueFlag = true;
        humanContinueFlag = false;
        //等待0.3s后继续执行，给玩家反应时间
        usleep(300000);
        //难度1，2，3搜索2，4，8层，终局搜索2，6，12层
        int difficultyDepthMap[3] = {2, 4, 8};
        int difficultyFinalDepthMap[3] = {2, 6, 12};
        ChessSituationNode::Node *node = new ChessSituationNode::Node(game->roundNum + 1, game->currentSituation->nextPlayer, game->currentSituation->chessboard);
        //思考落子位置
        MovingDecision movingDecision = aiAction->getMovingDecision(node, (game->roundNum >= 60 - difficultyFinalDepthMap[game->difficulty - 1]) ? difficultyFinalDepthMap[game->difficulty - 1] : difficultyDepthMap[game->difficulty - 1]);
        delete node;
        //思考完成时已经退出游戏界面，直接结束函数
        if(!isGameSceneEnabled){
            return false;
        }
        //落子信息
        Move *blackMove = new Move(movingDecision.row, movingDecision.col, game->blackPlayer);
        //执行落子
        game->playerMove(blackMove, game->currentSituation);
        delete blackMove;
        //更新棋谱信息
        ostringstream oss;
        oss << colNumCharacterTable[movingDecision.col] << movingDecision.row + 1;
        game->chessRecord += oss.str();
    }
    //游戏结束
    if (game->isGameOver(game->currentSituation)) {
        //某些按钮设置为可以使用
        undoButton->setEnabled(true);
        undoButton->setTitleColor(Color3B(255, 255, 255));
        //保存棋谱按钮激活
        saveChessRecordButton->setTitleColor(Color3B(255, 255, 255));
        saveChessRecordButton->setEnabled(true);
        //保存游戏按钮失效
        saveGameButton->setTitleColor(Color3B(150, 150, 150));
        saveGameButton->setEnabled(false);
        //信息标签显示为游戏结束
        if(game->getCurrentBlackPieceNum()>game->getCurrentWhitePieceNum()){
            infoLabel->setString("游戏结束\n  黑方胜");
        }else if(game->getCurrentBlackPieceNum()<game->getCurrentWhitePieceNum()){
            infoLabel->setString("游戏结束\n  白方胜");
        }else{
            infoLabel->setString("游戏结束\n    平局");
        }
        cout << "游戏结束" << endl;
//        cout << game->chessRecord << endl;
        //执行游戏结束操作
        game->gameEnd();
    }//否则
    else {
        //某些按钮设置为可以使用
        undoButton->setEnabled(true);
        undoButton->setTitleColor(Color3B(255, 255, 255));
        saveGameButton->setEnabled(true);
        saveGameButton->setTitleColor(Color3B(255, 255, 255));
        //轮到白子情况下信息标签显示
        //AI跳过
        if (humanContinueFlag && (game->currentSituation->nextPlayer != game->blackPlayer)) {
            infoLabel->setString("白子继续");
        }//否则
        else {
            humanContinueFlag = false;
            infoLabel->setString("轮到白子");
        }
    }
    aiContinueFlag = false;
    //打开棋盘点击响应
    isChessBoardEnabled = true;
    return true;
}

//白方AI操作
bool GameScene::whiteAIDo() {
    //关闭棋盘点击响应
    isChessBoardEnabled = false;
    //某些按钮设置为不可使用
    undoButton->setEnabled(false);
    undoButton->setTitleColor(Color3B(150, 150, 150));
    saveGameButton->setEnabled(false);
    saveGameButton->setTitleColor(Color3B(150, 150, 150));
    //信息标签显示为思考中
    infoLabel->setString("思考中...");
    //等待直到棋盘点击响应执行结束，等待0.3s，给玩家反应时间
//    usleep(300000);
    //在游戏没有结束且下一盘仍然是AI方的情况下继续落子
    while ((!(game->isGameOver(game->currentSituation))) && (game->currentSituation->nextPlayer == game->whitePlayer)) {
        if (aiContinueFlag) {
            infoLabel->setString("白子继续\n思考中...");
        }
        aiContinueFlag = true;
        humanContinueFlag = false;
        //等待0.3s后继续执行，给玩家反应时间
        usleep(300000);
        //难度1，2，3搜索2，4，8层，终局搜索2，6，12层
        int difficultyDepthMap[3] = {2, 4, 8};
        int difficultyFinalDepthMap[3] = {2, 6, 12};
        ChessSituationNode::Node *node = new ChessSituationNode::Node(game->roundNum + 1, game->currentSituation->nextPlayer, game->currentSituation->chessboard);
        //思考落子位置
        MovingDecision movingDecision = aiAction->getMovingDecision(node, (game->roundNum >= 60 - difficultyFinalDepthMap[game->difficulty - 1]) ? difficultyFinalDepthMap[game->difficulty - 1] : difficultyDepthMap[game->difficulty - 1]);
        delete node;
        //思考完成时已经退出游戏界面，直接结束函数
        if(!isGameSceneEnabled){
            return false;
        }
        //落子信息
        Move *whiteMove = new Move(movingDecision.row, movingDecision.col, game->whitePlayer);
        //执行落子
        game->playerMove(whiteMove, game->currentSituation);
        delete whiteMove;
        //更新棋谱信息
        ostringstream oss;
        oss << colNumCharacterTable[movingDecision.col] << movingDecision.row + 1;
        game->chessRecord += oss.str();
    }
    //游戏结束
    if (game->isGameOver(game->currentSituation)) {
        //某些按钮设置为可以使用
        undoButton->setEnabled(true);
        undoButton->setTitleColor(Color3B(255, 255, 255));
        //保存棋谱按钮激活
        saveChessRecordButton->setTitleColor(Color3B(255, 255, 255));
        saveChessRecordButton->setEnabled(true);
        //保存游戏按钮失效
        saveGameButton->setTitleColor(Color3B(150, 150, 150));
        saveGameButton->setEnabled(false);
        //信息标签显示为游戏结束
        if(game->getCurrentBlackPieceNum()>game->getCurrentWhitePieceNum()){
            infoLabel->setString("游戏结束\n  黑方胜");
        }else if(game->getCurrentBlackPieceNum()<game->getCurrentWhitePieceNum()){
            infoLabel->setString("游戏结束\n  白方胜");
        }else{
            infoLabel->setString("游戏结束\n    平局");
        }
        cout << "游戏结束" << endl;
//        cout << game->chessRecord << endl;
        //执行游戏结束操作
        game->gameEnd();
    }//否则
    else {
        //某些按钮设置为可以使用
        undoButton->setEnabled(true);
        undoButton->setTitleColor(Color3B(255, 255, 255));
        saveGameButton->setEnabled(true);
        saveGameButton->setTitleColor(Color3B(255, 255, 255));
        //轮到白子情况下信息标签显示
        //AI跳过
        if (humanContinueFlag && (game->currentSituation->nextPlayer != game->whitePlayer)) {
            infoLabel->setString("黑子继续");
        }//否则
        else {
            humanContinueFlag = false;
            infoLabel->setString("轮到黑子");
        }
    }
    aiContinueFlag = false;
    //打开棋盘点击响应
    isChessBoardEnabled = true;
    return true;
}

//保存游戏
void GameScene::saveGame() {
    //游戏模式
    if (game->mode == 1) {
        //人机
        UserDefault::getInstance()->setStringForKey("GameMode", "HumanToAI");
    } else if (game->mode == 2) {
        //双人
        UserDefault::getInstance()->setStringForKey("GameMode", "HumanToHuman");
    }

    //难度
    switch (game->difficulty) {
        case 1:
            UserDefault::getInstance()->setStringForKey("GameDifficulty", "Simple");
            break;
        case 2:
            UserDefault::getInstance()->setStringForKey("GameDifficulty", "Medium");
            break;
        case 3:
            UserDefault::getInstance()->setStringForKey("GameDifficulty", "Hard");
            break;
        default:
            //双人模式下
            UserDefault::getInstance()->setStringForKey("GameDifficulty", "NULL");
            break;
    }

    //玩家或AI谁先手
    //人机
    if(game->mode==1) {
        //玩家先手
        if (game->blackPlayer == game->aiAction->humanPlayer) {
            UserDefault::getInstance()->setStringForKey("PlayerOfFirstAction", "Human");
        }//AI先手
        else {
            UserDefault::getInstance()->setStringForKey("PlayerOfFirstAction", "AI");
        }
    }//双人
    else{
        UserDefault::getInstance()->setStringForKey("PlayerOfFirstAction", "Human");
    }

    //棋谱记录
    UserDefault::getInstance()->setStringForKey("ChessRecord", game->chessRecord);

    cout<<"游戏模式："<<UserDefault::getInstance()->getStringForKey("GameMode")<<endl;
    cout<<"难度："<<UserDefault::getInstance()->getStringForKey("GameDifficulty")<<endl;
    cout<<"先手："<<UserDefault::getInstance()->getStringForKey("PlayerOfFirstAction")<<endl;
    cout<<"棋谱："<<UserDefault::getInstance()->getStringForKey("ChessRecord")<<endl;

}

//保存棋谱
void GameScene::saveChessRecord(){
    ofstream out;
    out.open("/Users/Only/Desktop/棋谱.txt",ios::app);
    if(out){
        time_t rawTime;
        time(&rawTime);
        out<<ctime(&rawTime)<<game->chessRecord<<endl<<endl;
        out.close();
    }
}
