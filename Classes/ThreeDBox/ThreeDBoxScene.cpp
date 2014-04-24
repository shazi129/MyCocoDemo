#include "ThreeDBoxScene.h"
#include "../VisibleRect.h"

USING_NS_CC;

CCScene* ThreeDBoxScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    ThreeDBoxScene *layer = ThreeDBoxScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool ThreeDBoxScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
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
        menu_selector(ThreeDBoxScene::menuCloseCallback));

    pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
        origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    return true;
}

void ThreeDBoxScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->popScene();
}

void ThreeDBoxScene::testOpenGl()
{
    glLineWidth(1);
    ccDrawColor4B(255,255,255,255);
    ccPointSize(1);
    CHECK_GL_ERROR_DEBUG();

    ccDrawLine( VisibleRect::leftBottom(), VisibleRect::rightTop());
    CHECK_GL_ERROR_DEBUG();

    glLineWidth(5.0f);
    ccDrawColor4B(255, 0, 0, 255);
    ccDrawLine( VisibleRect::rightBottom(), VisibleRect::leftTop());
    CHECK_GL_ERROR_DEBUG();

    ccPointSize(64);
    ccDrawColor4B(0,0,255,128);
    ccDrawPoint( VisibleRect::center() );
    CHECK_GL_ERROR_DEBUG();

    ccDrawColor4B(255, 255, 0, 255);
    glLineWidth(10);
    CCPoint points[] = { ccp(0,0), ccp(50,50), ccp(100,50), ccp(100,100), ccp(50,100) };
    ccDrawPoly( points, 5, false);
    CHECK_GL_ERROR_DEBUG();

    glLineWidth(2);
    ccDrawColor4B(0, 255, 255, 255);
    ccDrawCircle( VisibleRect::center(), 100, CC_DEGREES_TO_RADIANS(1), 50, false);
    CHECK_GL_ERROR_DEBUG();
}

void ThreeDBoxScene::testThreeDBox()
{
    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
    setShaderProgram(program);

    CCTexture2D * mTexture = CCTextureCache::sharedTextureCache()->addImage("HelloWorld.png");
    CCDirector::sharedDirector()->setDepthTest(true);
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
    getShaderProgram()->use();
    ccGLBindTexture2D( mTexture->getName() );

    ccVertex3F vertices[4];
    ccVertex2F uv[4];

    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, uv);


    float x = 0;
    float y = 0;
    float len = 8;

    /////////// front
    vertices[0] = vertex3(x-len,y-len,len);                
    vertices[1] = vertex3(x-len,y+len,len);
    vertices[2] = vertex3(x+len,y-len,len);
    vertices[3] = vertex3(x+len,y+len,len);

    uv[0] = vertex2(0, 1);
    uv[1] = vertex2(0, 0);
    uv[2] = vertex2(1, 1);
    uv[3] = vertex2(1, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    ////////// right
    vertices[0] = vertex3(x+len,y-len,len);                        
    vertices[1] = vertex3(x+len,y+len,len);
    vertices[2] = vertex3(x+len,y-len,-len);
    vertices[3] = vertex3(x+len,y+len,-len);

    uv[0] = vertex2(0, 1);
    uv[1] = vertex2(0, 0);
    uv[2] = vertex2(1, 1);
    uv[3] = vertex2(1, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    ///////// back
    vertices[0] = vertex3(x+len,y-len,-len);        
    vertices[1] = vertex3(x+len,y+len,-len);
    vertices[2] = vertex3(x-len,y-len,-len);
    vertices[3] = vertex3(x-len,y+len,-len);

    uv[0] = vertex2(0, 1);
    uv[1] = vertex2(0, 0);
    uv[2] = vertex2(1, 1);
    uv[3] = vertex2(1, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    ////////// left
    vertices[0] = vertex3(x-len,y-len,len);                        
    vertices[1] = vertex3(x-len,y+len,len);
    vertices[2] = vertex3(x-len,y-len,-len);
    vertices[3] = vertex3(x-len,y+len,-len);

    uv[0] = vertex2(0, 1);
    uv[1] = vertex2(0, 0);
    uv[2] = vertex2(1, 1);
    uv[3] = vertex2(1, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


    ///////// top
    vertices[0] = vertex3(x+len,y+len,len);        
    vertices[1] = vertex3(x-len,y+len,len);
    vertices[2] = vertex3(x+len,y+len,-len);
    vertices[3] = vertex3(x-len,y+len,-len);

    uv[0] = vertex2(0, 0);
    uv[1] = vertex2(1, 0);
    uv[2] = vertex2(0, 1);
    uv[3] = vertex2(1, 1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    ///////// bottom
    vertices[0] = vertex3(x+len,y-len,len);        
    vertices[1] = vertex3(x-len,y-len,len);
    vertices[2] = vertex3(x+len,y-len,-len);
    vertices[3] = vertex3(x-len,y-len,-len);

    uv[0] = vertex2(0, 0);
    uv[1] = vertex2(1, 0);
    uv[2] = vertex2(0, 1);
    uv[3] = vertex2(1, 1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    kmMat4 matrixP;
    kmMat4 matrixMV;
    kmMat4 matrixMVP;

    kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
    kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );

    kmQuaternion quat;
    kmQuaternionRotationYawPitchRoll(&quat, 10, 10, 10); // 1

    kmMat3 rotation;
    kmMat3RotationQuaternion(&rotation, &quat);  // 2

    kmVec3 translation;
    kmVec3Fill(&translation, 240, 150, 220);  // 3

    kmMat4 rotationAndMove;
    kmMat4RotationTranslation(&rotationAndMove, &rotation, &translation);  // 4

    kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
    kmMat4Multiply(&matrixMVP, &matrixMVP, &rotationAndMove);        // 5

    GLuint matrixId = glGetUniformLocation(getShaderProgram()->getProgram(), "u_MVPMatrix");
    getShaderProgram()->setUniformLocationWithMatrix4fv(matrixId, matrixMVP.mat, 1); // 6
}

void ThreeDBoxScene::draw()
{
    CCLayer::draw();
    CHECK_GL_ERROR_DEBUG();

    float x = 0;
    float y = 0;
    float len = 8;

    ccVertex3F vertices[4];
    ccVertex2F uv[4];

    /////////// front
    vertices[0] = vertex3(100,100,len);                
    vertices[1] = vertex3(100,200,len);
    vertices[2] = vertex3(200,200,len);
    vertices[3] = vertex3(200,100,len);

    uv[0] = vertex2(0, 1);
    uv[1] = vertex2(0, 0);
    uv[2] = vertex2(1, 1);
    uv[3] = vertex2(1, 0);

    ccGLBindTexture2D(CCTextureCache::sharedTextureCache()->addImage("HelloWorld.png")->getName());
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);

    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, uv);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}