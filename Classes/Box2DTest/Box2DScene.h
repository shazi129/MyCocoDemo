#ifndef _BOX_2D_SCENE_H_
#define _BOX_2D_SCENE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#define PTM_RATIO 0.05

class Box2DScene : public cocos2d::CCLayer
{
public:
    Box2DScene();
    ~Box2DScene();

    static cocos2d::CCScene * scene();
    virtual bool init();

    void menuCloseCallback(CCObject* pSender);
    void initBox2D();

    CREATE_FUNC(Box2DScene);

    virtual void update(float delta);

private:
    b2World * mWorld;
    b2Body * mBallBody;
};
#endif