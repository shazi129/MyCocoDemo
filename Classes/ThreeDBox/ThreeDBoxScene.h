#ifndef _3D_BOX_SCENE_H_
#define _3D_BOX_SCENE_H_

#include "cocos2d.h"

class ThreeDBoxScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    void menuCloseCallback(CCObject* pSender);
    CREATE_FUNC(ThreeDBoxScene);

    virtual void draw();
    void testOpenGl();
    void testThreeDBox();

};

#endif