#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "ui/UIRichText.h"
#include <iostream>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class MainScene : public Scene
{
private:
    int difficulty;
    bool humanFirst;
    Label *titleLabel;
    Button *resumeGameButton;
    void createBackground();
    void createTitle();
    void createMainOptionsLayer();
    void createChooseDificultyOptionsLayer();
    void createChooseColorLayer();
    void createHelpLayer();
    void switchToDoublePersonGameScene();
    void switchToHumanToAIGameScene();
    void loadGame();                                            //加载游戏
public:
    static Scene* createScene();

    virtual bool init();
    virtual void onEnter();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
    
    Layer *mainOptionsLayer;
    Layer *chooseDificultyOptionsLayer;
    Layer *chooseColorLayer;
    Layer *helpLayer;
    
};

#endif // __MAIN_SCENE_H__
