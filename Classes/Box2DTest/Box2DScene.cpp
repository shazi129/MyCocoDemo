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

    CCSprite * ball = CCSprite::create("CloseNormal.png");
    ball->setAnchorPoint(ccp(0.5, 0.5));
    ball->setPosition(VisibleRect::center());
    this->addChild(ball);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData = ball;
    bodyDef.linearDamping = 0.0f;
    bodyDef.position.Set(ball->getPositionX() * PTM_RATIO, ball->getPositionY()* PTM_RATIO);
    mBallBody = mWorld->CreateBody(&bodyDef);

    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(20* PTM_RATIO, 20* PTM_RATIO);

    b2FixtureDef bodyFixture;
    bodyFixture.density = 1;
    bodyFixture.shape = &bodyShape;
    bodyFixture.friction = 0.1f;
    bodyFixture.restitution = 1.0f;
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
    gravity.Set(0.0f, -10.0f);
    mWorld = new b2World(gravity);

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    b2PolygonShape borderShape;
    b2FixtureDef borderFixture;
    borderFixture.density = 1;

    //µØ°å
    bodyDef.position.Set(0, 0);
    borderShape.SetAsBox(VisibleRect::right().x * PTM_RATIO, 0);
    borderFixture.shape = &borderShape;
    b2Body * bottomBorder = mWorld->CreateBody(&bodyDef);
    bottomBorder->CreateFixture(&borderFixture);

}

void Box2DScene::menuCloseCallback(CCObject* pSender)
{
    unscheduleUpdate();
    CCDirector::sharedDirector()->popScene();
}