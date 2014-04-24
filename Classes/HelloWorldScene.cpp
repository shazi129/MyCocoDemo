#include "HelloWorldScene.h"
#include "ThreeDBox/ThreeDBoxScene.h"
#include "Box2DTest/Box2DScene.h"
#include "VisibleRect.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::~HelloWorld()
{
    menuArray->release();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    //
    menuArray = new CCArray();
    menuArray->retain();
    menuArray->addObject(CCString::createWithFormat("3DBox Test"));
    menuArray->addObject(CCString::createWithFormat("Box2D Test"));

    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
    pCloseItem->setAnchorPoint(ccp(1, 0));
	pCloseItem->setPosition(VisibleRect::rightBottom());

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
 
    // add "HelloWorld" splash screen"
    CCSprite* background = CCSprite::create("star.png");
    background->setAnchorPoint(ccp(0.5, 0.5));
    background->setPosition(VisibleRect::center());
    this->addChild(background, 0);

    CCTableView *tableView=CCTableView::create(this, CCSizeMake(300, visibleSize.height));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setAnchorPoint(ccp(0.5, 0));
    tableView->setPosition(ccp(visibleSize.width/2 + origin.x, 0));
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView,1);
    tableView->reloadData();

    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("you touch cell index = %u",cell->getIdx());
    switch(cell->getIdx())
    {
    case 0: //ThreeDBox
        {
            CCDirector::sharedDirector()->pushScene(ThreeDBoxScene::scene());
        }
        break;
    case 1:
        {
            CCDirector::sharedDirector()->pushScene(Box2DScene::scene());
        }
        break;
    default:
        break;
    }
}

unsigned int HelloWorld::numberOfCellsInTableView(CCTableView *table)
{
    if (menuArray)
    {
        return menuArray->count();
    }
    return 0;
}

CCTableViewCell* HelloWorld::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCLOG("int table cell index:%u",idx);

    CCTableViewCell *cell = table->dequeueCell();
    if (!cell)
    {
        cell = new CCTableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildrenWithCleanup(true);

    if (menuArray->count() < idx + 1)
    {
        return NULL;
    }

    CCString * text = (CCString *)(menuArray->objectAtIndex(idx));
    CCLabelTTF * menuText = CCLabelTTF::create(text->getCString(), "", 24);
    menuText->setAnchorPoint(ccp(0, 0));
    menuText->setPosition(ccp(0, 0));
    cell->addChild(menuText);

    return cell;
}

CCSize HelloWorld::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(200, 50);
}

void HelloWorld::scrollViewDidScroll(CCScrollView* view)
{
    CCLOG("scrollViewDidScroll");
}
void HelloWorld::scrollViewDidZoom(CCScrollView* view)
{
    CCLOG("scrollViewDidZoom");
}
