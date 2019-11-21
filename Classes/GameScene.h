#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "GameLogic/Logic/Game.h"
#include "Algorithm/AIAction/Node.h"
#include "Algorithm/AIAction/MovingDecision.h"
#include "Algorithm/AIAction/AIAction.h"
#include <iostream>
#include <unistd.h>
#include <thread>
#include <fstream>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class GameScene:public Scene{
private:
    void createInfoLayer();
    void createOptionsLayer();
    void createChessBoardLayer();
    void gameOverDo();
    bool onChessBoardTouchBegan(Touch *touch,Event *event);
//    bool onChessBoardTouchEnded(Touch *touch,Event *event);
    bool blackHumanDo(int row,int col);
    bool whiteHumanDo(int row,int col);
    bool blackAIDo();
    bool whiteAIDo();
public:
    
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);
    
    void update(float dt);
    
    //图层
    Layer *chessBoardLayer;
    Layer *optionsLayer;
    Layer *infoLayer;
    
    //按钮
    Button *returnMainButton;
    Button *undoButton;
    Button *saveGameButton;
    Button *saveChessRecordButton;
    Button *startButton;
    
    //标签
    Label *infoLabel;
    Label *playerLabel;
    Label *scoreLabel;
    
    
    //精灵
    Sprite *chessBoardSprite;
    Sprite *chessPieceSprite[8][8];
    bool isChessBoardEnabled;

    //游戏类
    Game *game;
    AIAction *aiAction;
    string colNumCharacterTable="ABCDEFGH";
    bool aiContinueFlag=false;
    bool humanContinueFlag=false;
    bool isGameSceneEnabled;                                    //游戏场景是否可用，应对思考中退出问题
    void paintChessBoard();
    void updateInterface();
    void updateInfoLayer();
    void saveGame();                                            //保存游戏
    void saveChessRecord();
//    bool humanFirst;

};

#endif /* GameScene_h */
