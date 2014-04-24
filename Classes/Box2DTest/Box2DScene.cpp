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

    CCSprite * ball = CCSprite::create("CloseNormal.png"); //С���Ӧ�ľ���
    ball->setAnchorPoint(ccp(0.5, 0.5));
    ball->setPosition(VisibleRect::center());
    this->addChild(ball);

    bodyDef.type = b2_dynamicBody; //С���ǿ��Զ���
    bodyDef.userData = ball;    //��ֻ��Ϊ�˱������ݣ�����ͨ�������ҵ���Ӧ�ľ���
    bodyDef.linearDamping = 0.0f; //���ٶ�Ϊ0
    bodyDef.position.Set(ball->getPositionX() * PTM_RATIO, ball->getPositionY()* PTM_RATIO); //��ʼλ��
    mBallBody = mWorld->CreateBody(&bodyDef); // ��Box2D�����д���һ������

    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(20* PTM_RATIO, 20* PTM_RATIO); //С��Ĵ�С����״~ ���εģ�����Բ�ε�Ӧ����b2CircleShape

    b2FixtureDef bodyFixture; //С��Ķ�����
    bodyFixture.density = 1;  // �ܶ�
    bodyFixture.shape = &bodyShape; //��״
    bodyFixture.friction = 0.1f; //Ħ��������Ϊ���������û��������Ħ��������û��
    bodyFixture.restitution = 1.0f; //������100%����~~~
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
    gravity.Set(0.0f, -10.0f); //һ������10��λ����������Ϊ�������ٶȣ�Box2D��Ĭ�ϵĵ�λ�������
    mWorld = new b2World(gravity); // ����һ�����������ٶȵ�����

    b2BodyDef bodyDef;   //һ������Ķ���
    bodyDef.type = b2_staticBody; //��������ͣ������ģ���Ӳ��~ ���԰����������嵱�ɵ���
    b2PolygonShape borderShape; // һ�����ε���״
    b2FixtureDef borderFixture; // һ��������

    //����ذ�
    bodyDef.position.Set(0, 0); // �ذ���0��0λ��
    borderShape.SetAsBox(VisibleRect::right().x * PTM_RATIO, 0); //���ó��Ⱥ͸߶ȣ����������ĵذ������X�ᣬ��һ����
    borderFixture.shape = &borderShape;
    b2Body * bottomBorder = mWorld->CreateBody(&bodyDef); //�������д���һ�����壨���棩
    bottomBorder->CreateFixture(&borderFixture);          //���Ƶ������״

}

void Box2DScene::menuCloseCallback(CCObject* pSender)
{
    unscheduleUpdate();
    CCDirector::sharedDirector()->popScene();
}