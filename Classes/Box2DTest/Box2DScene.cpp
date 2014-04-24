#include "Box2DScene.h"
#include "../VisibleRect.h"

USING_NS_CC;

Box2DScene::Box2DScene()
    : mWorld(NULL),
      mBallBody(NULL)
{
}

Box2DScene::~Box2DScene()
{
}

CCScene * Box2DScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);

        CCLayer * box2DLayer = Box2DScene::create();
        CC_BREAK_IF(!box2DLayer);

        scene->addChild(box2DLayer);
    } while (0);
    
    return scene;
}

bool Box2DScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    initBox2D();

    //close menu
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        this,
        menu_selector(Box2DScene::menuCloseCallback));

    pCloseItem->setAnchorPoint(ccp(1, 0));
    pCloseItem->setPosition(VisibleRect::rightBottom());

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    b2BodyDef bodyDef;

    CCSprite * ball = CCSprite::create("CloseNormal.png"); //小球对应的精灵
    ball->setAnchorPoint(ccp(0.5, 0.5));
    ball->setPosition(VisibleRect::center());
    this->addChild(ball);

    bodyDef.type = b2_dynamicBody; //小球是可以动的
    bodyDef.userData = ball;    //这只是为了保存数据，可以通过物体找到对应的精灵
    bodyDef.linearDamping = 0.0f; //初速度为0
    bodyDef.position.Set(ball->getPositionX() * PTM_RATIO, ball->getPositionY()* PTM_RATIO); //初始位置
    mBallBody = mWorld->CreateBody(&bodyDef); // 在Box2D世界中创建一个物体

    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(20* PTM_RATIO, 20* PTM_RATIO); //小球的大小和形状~ 方形的？？？圆形的应该是b2CircleShape

    b2FixtureDef bodyFixture; //小球的定制器
    bodyFixture.density = 1;  // 密度
    bodyFixture.shape = &bodyShape; //形状
    bodyFixture.friction = 0.1f; //摩擦力，因为掉落过程中没有物体间的摩擦，所以没用
    bodyFixture.restitution = 1.0f; //弹力，100%反弹~~~
    mBallBody->CreateFixture(&bodyFixture);

    this->scheduleUpdate();
    return true;
}

void Box2DScene::update(float delta)
{
    int32 velocityIterations = 10;
    int32 positionIterations = 10;

    mWorld->Step(delta, velocityIterations, positionIterations);
    mWorld->ClearForces();

    CCSprite * ball = (CCSprite *)(mBallBody->GetUserData());
    b2Vec2 ballPosition = mBallBody->GetPosition();
    ball->setPosition(ccp(ballPosition.x / PTM_RATIO, ballPosition.y / PTM_RATIO));

    CCLOG("update, delta=%f, x=%f, y=%f, v=%f",delta, ballPosition.x, ballPosition.y, mBallBody->GetLinearVelocity().y);
}

void Box2DScene::initBox2D()
{
    // init world
    b2Vec2 gravity; 
    gravity.Set(0.0f, -10.0f); //一个向下10单位的向量，作为重力减速度，Box2D中默认的单位是秒和米
    mWorld = new b2World(gravity); // 创建一个有重力加速度的世界

    b2BodyDef bodyDef;   //一个物体的定义
    bodyDef.type = b2_staticBody; //物体的类型，不动的，坚硬的~ 可以把这样的物体当成地面
    b2PolygonShape borderShape; // 一个方形的形状
    b2FixtureDef borderFixture; // 一个定制器

    //定义地板
    bodyDef.position.Set(0, 0); // 地板在0，0位置
    borderShape.SetAsBox(VisibleRect::right().x * PTM_RATIO, 0); //设置长度和高度，这里描述的地板就像是X轴，是一条线
    borderFixture.shape = &borderShape;
    b2Body * bottomBorder = mWorld->CreateBody(&bodyDef); //在世界中创建一个物体（地面）
    bottomBorder->CreateFixture(&borderFixture);          //定制地面的形状

}

void Box2DScene::menuCloseCallback(CCObject* pSender)
{
    unscheduleUpdate();
    CCDirector::sharedDirector()->popScene();
}