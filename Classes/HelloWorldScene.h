#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class HelloWorld : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    ~HelloWorld();

    //implement table view delegate
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell); // on table touched

    //implement table view data Source
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table); // number of cells
    virtual CCSize cellSizeForTable(CCTableView *table);

    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);

private:
    CCArray * menuArray;

};

#endif // __HELLOWORLD_SCENE_H__
