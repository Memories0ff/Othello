#include "MainScene.h"

Scene *MainScene::createScene() {
    return MainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool MainScene::init() {
    if (!Scene::init()) {
        return false;
    }

    createBackground();
    createTitle();
    createMainOptionsLayer();
    createChooseDificultyOptionsLayer();
    createChooseColorLayer();
    createHelpLayer();

    return true;
}

void MainScene::onEnter(){
    Scene::onEnter();
//    保存了游戏则激活恢复游戏按钮
    if (UserDefault::getInstance()->getStringForKey("GameMode") != "") {
        resumeGameButton->setTitleColor(Color3B(255, 255, 255));
        resumeGameButton->setEnabled(true);
    }
}

//背景图片
void MainScene::createBackground() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto backgroundSprite = Sprite::create("wooden_background.jpg");
    backgroundSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    auto picRect = backgroundSprite->getTextureRect();
    backgroundSprite->setScale(visibleSize.width / picRect.getMaxX(), visibleSize.height / picRect.getMaxY());
    this->addChild(backgroundSprite);
}

//标题Othello
void MainScene::createTitle() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    titleLabel = Label::createWithTTF("Othello", "fonts/arial.ttf", 56);
    titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 10 * 7));
    this->addChild(titleLabel);
}

//主菜单
void MainScene::createMainOptionsLayer() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    //主菜单图层
    mainOptionsLayer = Layer::create();
    mainOptionsLayer->setTag(1);
    this->addChild(mainOptionsLayer);

    //恢复游戏按钮
    resumeGameButton = Button::create();
    resumeGameButton->setTitleText("恢复游戏");
    resumeGameButton->setTitleFontSize(19);
    resumeGameButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.4));
    //之前没有保存的游戏则按钮失效
    if (UserDefault::getInstance()->getStringForKey("GameMode") == "") {
        resumeGameButton->setTitleColor(Color3B(200, 200, 200));
        resumeGameButton->setEnabled(false);
    }
    resumeGameButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Resume game" << endl;
                loadGame();
                break;
            default:
                break;
        }
    });
    mainOptionsLayer->addChild(resumeGameButton);

    //单人游戏按钮
    auto singlePersonGameButton = Button::create();
    singlePersonGameButton->setTitleText("单人游戏");
    singlePersonGameButton->setTitleFontSize(19);
    singlePersonGameButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.325));
    singlePersonGameButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Single person game" << endl;
                mainOptionsLayer->setVisible(false);
                chooseDificultyOptionsLayer->setVisible(true);
                break;
            default:
                break;
        }
    });
    mainOptionsLayer->addChild(singlePersonGameButton);

    //双人游戏按钮
    auto doublePersonGameButton = Button::create();
    doublePersonGameButton->setTitleText("双人游戏");
    doublePersonGameButton->setTitleFontSize(19);
    doublePersonGameButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.25));
    doublePersonGameButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Double person game" << endl;
                switchToDoublePersonGameScene();
                break;
            default:
                break;
        }
    });
    mainOptionsLayer->addChild(doublePersonGameButton);

    //帮助按钮
    auto helpButton = Button::create();
    helpButton->setTitleText("游戏帮助");
    helpButton->setTitleFontSize(19);
    helpButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.175));
    helpButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Game help" << endl;
                titleLabel->setVisible(false);
                mainOptionsLayer->setVisible(false);
                helpLayer->setVisible(true);
                break;
            default:
                break;
        }
    });
    mainOptionsLayer->addChild(helpButton);

    //退出按钮
    auto exitButton = Button::create();
    exitButton->setTitleText("退出游戏");
    exitButton->setTitleFontSize(19);
    exitButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.1));
    exitButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Exit game" << endl;
                Director::getInstance()->end();
                exit(0);
                break;
            default:
                break;
        }
    });
    mainOptionsLayer->addChild(exitButton);

}

//难度选项
void MainScene::createChooseDificultyOptionsLayer() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    //难度选择图层
    chooseDificultyOptionsLayer = Layer::create();
    chooseDificultyOptionsLayer->setTag(2);
    chooseDificultyOptionsLayer->setVisible(false);
    this->addChild(chooseDificultyOptionsLayer);

    //返回按钮
    auto returnMainButton = Button::create();
    returnMainButton->setTitleText("返回");
    returnMainButton->setTitleFontSize(19);
    returnMainButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.4));
    returnMainButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Return main" << endl;
                chooseDificultyOptionsLayer->setVisible(false);
                mainOptionsLayer->setVisible(true);
                break;
            default:
                break;
        }
    });
    chooseDificultyOptionsLayer->addChild(returnMainButton);

    //简单按钮
    auto easyButton = Button::create();
    easyButton->setTitleText("简单");
    easyButton->setTitleFontSize(19);
    easyButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.3));
    easyButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Easy" << endl;
                difficulty = 1;
                chooseDificultyOptionsLayer->setVisible(false);
                chooseColorLayer->setVisible(true);
                break;
            default:
                break;
        }
    });
    chooseDificultyOptionsLayer->addChild(easyButton);

    //中等按钮
    auto mediumButton = Button::create();
    mediumButton->setTitleText("中等");
    mediumButton->setTitleFontSize(19);
    mediumButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.2));
    mediumButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Medium" << endl;
                difficulty = 2;
                chooseDificultyOptionsLayer->setVisible(false);
                chooseColorLayer->setVisible(true);
                break;
            default:
                break;
        }
    });
    chooseDificultyOptionsLayer->addChild(mediumButton);

    //困难按钮
    auto hardButton = Button::create();
    hardButton->setTitleText("困难");
    hardButton->setTitleFontSize(19);
    hardButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.1));
    hardButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Hard" << endl;
                difficulty = 3;
                chooseDificultyOptionsLayer->setVisible(false);
                chooseColorLayer->setVisible(true);
                break;
            default:
                break;
        }
    });
    chooseDificultyOptionsLayer->addChild(hardButton);

}

//玩家选择棋子颜色
void MainScene::createChooseColorLayer() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    //选择棋子颜色按钮
    chooseColorLayer = Layer::create();
    chooseColorLayer->setTag(3);
    chooseColorLayer->setVisible(false);
    this->addChild(chooseColorLayer);


    //返回按钮
    auto returnChooseDificultyOptionsButton = Button::create();
    returnChooseDificultyOptionsButton->setTitleText("返回");
    returnChooseDificultyOptionsButton->setTitleFontSize(19);
    returnChooseDificultyOptionsButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.4));
    returnChooseDificultyOptionsButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Return choose difficulty" << endl;
                chooseColorLayer->setVisible(false);
                chooseDificultyOptionsLayer->setVisible(true);
                break;
            default:
                break;
        }
    });
    chooseColorLayer->addChild(returnChooseDificultyOptionsButton);

    //黑方
    auto blackButton = Button::create();
    blackButton->setTitleText("黑方");
    blackButton->setTitleFontSize(19);
    blackButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.25));
    blackButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Black" << endl;
                humanFirst = true;
                switchToHumanToAIGameScene();
                break;
            default:
                break;
        }
    });
    chooseColorLayer->addChild(blackButton);

    //白方
    auto whiteButton = Button::create();
    whiteButton->setTitleText("白方");
    whiteButton->setTitleFontSize(19);
    whiteButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.1));
    whiteButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "White" << endl;
                humanFirst = false;
                switchToHumanToAIGameScene();
                break;
            default:
                break;
        }
    });
    chooseColorLayer->addChild(whiteButton);

}

//帮助信息
void MainScene::createHelpLayer() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    helpLayer = Layer::create();
    helpLayer->setTag(4);
    helpLayer->setVisible(false);
    this->addChild(helpLayer);

    //帮助信息富文本
    RichText *helpRichText = RichText::create();
    helpRichText->ignoreContentAdaptWithSize(false);
    helpRichText->setSize(Size(visibleSize.width * 3 / 4, visibleSize.height * 3 / 4));
    helpRichText->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    RichElementText *helpRichElemText = RichElementText::create(1, Color3B(255, 255, 255), 255, "【简介】\n"
                                                                                                "黑白棋又叫反棋(Reversi)、奥赛罗棋(Othello)、苹果棋或翻转棋。游戏通过相互翻转对方的棋子，最后以棋盘上谁的棋子多来判断胜负。\n"
                                                                                                "【规则】\n"
                                                                                                "1．黑方先行，双方交替下棋。\n"
                                                                                                "2．新落下的棋子与棋盘上已有的同色棋子间，对方被夹住的所有棋子都要翻转过来。可以是横着夹，竖着夹，或是斜着夹。夹住的位置上必须全部是对手的棋子，不能有空格。\n"
                                                                                                "3．新落下的棋子必须翻转对手一个或多个棋子，否则就不能落子。\n"
                                                                                                "4．如果一方没有合法棋步，也就是说不管他下到哪里，都不能至少翻转对手的一个棋子，那他这一轮只能弃权，而由他的对手继续落子直到他有合法棋步可下。\n"
                                                                                                "5．如果一方至少有一步合法棋步可下，他就必须落子，不得弃权。\n"
                                                                                                "6．当棋盘填满或者双方都无合法棋步可下时，游戏结束。结束时谁的棋子最多谁就是赢家。", "fonts/wqy-microhei.ttc", 12);
    helpRichText->pushBackElement(helpRichElemText);

    helpLayer->addChild(helpRichText);

    //返回按钮
    auto returnMainButton = Button::create();
    returnMainButton->setTitleText("返回");
    returnMainButton->setTitleFontSize(19);
    returnMainButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.1));
    returnMainButton->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                break;
            case Widget::TouchEventType::ENDED:
                cout << "Return main" << endl;
                helpLayer->setVisible(false);
                mainOptionsLayer->setVisible(true);
                titleLabel->setVisible(true);
                break;
            default:
                break;
        }
    });
    helpLayer->addChild(returnMainButton);

}

//选择双人游戏
void MainScene::switchToDoublePersonGameScene() {
    GameScene *gameScene = static_cast<GameScene *>(GameScene::createScene());
    gameScene->game = Game::gameStartForHumanToHuman();
    gameScene->startButton->setTitleText("游戏已经开始");
    gameScene->startButton->setTitleColor(Color3B(150, 150, 150));
    gameScene->startButton->setEnabled(false);
    gameScene->paintChessBoard();
    Director::getInstance()->pushScene(gameScene);
    mainOptionsLayer->setVisible(true);
    chooseDificultyOptionsLayer->setVisible(false);
    chooseColorLayer->setVisible(false);
    //界面按帧更新
    gameScene->scheduleUpdate();
}

//选择人机对战
void MainScene::switchToHumanToAIGameScene() {
    GameScene *gameScene = static_cast<GameScene *>(GameScene::createScene());
    if (humanFirst) {
        gameScene->game = Game::gameStartForHumanToAI(difficulty, humanFirst);
        gameScene->aiAction = gameScene->game->aiAction;                      //AI白
        gameScene->startButton->setTitleText("游戏已经开始");
        gameScene->startButton->setTitleColor(Color3B(150, 150, 150));
        gameScene->startButton->setEnabled(false);
    } else {
        gameScene->game = Game::gameStartForHumanToAI(difficulty, humanFirst);
        gameScene->aiAction = gameScene->game->aiAction;                      //AI黑
        gameScene->isChessBoardEnabled = false;
    }
    gameScene->paintChessBoard();
    Director::getInstance()->pushScene(gameScene);
    mainOptionsLayer->setVisible(true);
    chooseDificultyOptionsLayer->setVisible(false);
    chooseColorLayer->setVisible(false);
    //界面按帧更新
    gameScene->scheduleUpdate();
}

//加载游戏
void MainScene::loadGame() {
    cout << "游戏模式：" << UserDefault::getInstance()->getStringForKey("GameMode") << endl;
    cout << "难度：" << UserDefault::getInstance()->getStringForKey("GameDifficulty") << endl;
    cout << "先手：" << UserDefault::getInstance()->getStringForKey("PlayerOfFirstAction") << endl;
    cout << "棋谱：" << UserDefault::getInstance()->getStringForKey("ChessRecord") << endl;

    //游戏模式
    int mode;
    if (UserDefault::getInstance()->getStringForKey("GameMode") == "HumanToAI") {
        mode = 1;
    } else if (UserDefault::getInstance()->getStringForKey("GameMode") == "HumanToHuman") {
        mode = 2;
    }

    //难度
    int difficulty;
    if (UserDefault::getInstance()->getStringForKey("GameDifficulty") == "Simple") {
        difficulty = 1;
    } else if (UserDefault::getInstance()->getStringForKey("GameDifficulty") == "Medium") {
        difficulty = 2;
    } else if (UserDefault::getInstance()->getStringForKey("GameDifficulty") == "Hard") {
        difficulty = 3;
    } else {
        difficulty = 0;
    }

    //先手
    bool humanFirst;
    if (UserDefault::getInstance()->getStringForKey("PlayerOfFirstAction") == "Human") {
        humanFirst = true;
    } else if (UserDefault::getInstance()->getStringForKey("PlayerOfFirstAction") == "AI") {
        humanFirst = false;
    }

    //棋谱
    string chessRecord = UserDefault::getInstance()->getStringForKey("ChessRecord");

    GameScene *gameScene = static_cast<GameScene *>(GameScene::createScene());

    gameScene->game = Game::loadGame(difficulty, humanFirst, mode, chessRecord);
    gameScene->aiAction = gameScene->game->aiAction;

    //失效开始按钮
    gameScene->startButton->setTitleText("游戏已经开始");
    gameScene->startButton->setTitleColor(Color3B(150, 150, 150));
    gameScene->startButton->setEnabled(false);
    //失效保存游戏按钮
    gameScene->saveGameButton->setTitleColor(Color3B(150, 150, 150));
    gameScene->saveGameButton->setEnabled(false);

    //设置消息框
    if (gameScene->game->currentSituation->nextPlayer == gameScene->game->blackPlayer) {
        gameScene->infoLabel->setString("轮到黑子");
    } else {
        gameScene->infoLabel->setString("轮到白子");
    }

    //设置悔棋按钮可用
    if (gameScene->game->mode == 1 && humanFirst && gameScene->game->roundNum > 1) {
        gameScene->undoButton->setTitleColor(Color3B(255, 255, 255));
        gameScene->undoButton->setEnabled(true);
    } else if (gameScene->game->mode == 1 && !humanFirst && gameScene->game->roundNum > 2) {
        gameScene->undoButton->setTitleColor(Color3B(255, 255, 255));
        gameScene->undoButton->setEnabled(true);
    } else if (gameScene->game->mode == 2 && gameScene->game->roundNum > 1) {
        gameScene->undoButton->setTitleColor(Color3B(255, 255, 255));
        gameScene->undoButton->setEnabled(true);
    }

    gameScene->paintChessBoard();
    Director::getInstance()->pushScene(gameScene);
    mainOptionsLayer->setVisible(true);
    chooseDificultyOptionsLayer->setVisible(false);
    chooseColorLayer->setVisible(false);
    //界面按帧更新
    gameScene->scheduleUpdate();
}