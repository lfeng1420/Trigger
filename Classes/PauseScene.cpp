#include "PauseScene.h"
#include "BarrierLayer.h"
#include "GameScene.h"
#include "DataManager.h"
#include "ThemeScene.h"

USING_NS_CC;

CPauseScene::CPauseScene()
{

}

CPauseScene::~CPauseScene()
{

}

bool CPauseScene::init()
{
	if(!Layer::init())
	{
		return false;
	}

	float fCurX = 0;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float fScreenHeight = visibleSize.height;
	float fHalfWidth = visibleSize.width / 2;

	auto pauseSprite = Sprite::create("images/gamepause.png");
	pauseSprite->setPosition(fHalfWidth, fScreenHeight * 0.6f);
	addChild(pauseSprite);

	//重新开始按钮
	auto restartButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_finish_restart.png"),
		Sprite::createWithSpriteFrameName("btn_finish_restart.png"),
		CC_CALLBACK_1(CPauseScene::OnMenuCallback, this, 0)
	);
	float fBtnWidth = restartButton->getContentSize().width;
	float fBtnY = fScreenHeight * 0.4f;
	float fPadding = (fHalfWidth * 2 - 3 * fBtnWidth) / 4;
	restartButton->setPosition(fPadding + fBtnWidth / 2, fBtnY);
	
	//继续按钮
	auto resumeButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_finish_next.png"),
		Sprite::createWithSpriteFrameName("btn_finish_next.png"),
		CC_CALLBACK_1(CPauseScene::OnMenuCallback, this, 1)
	);
	resumeButton->setPosition(fHalfWidth, fBtnY);
	//菜单按钮
	auto menuButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_finish_menu.png"),
		Sprite::createWithSpriteFrameName("btn_finish_menu.png"),
		CC_CALLBACK_1(CPauseScene::OnMenuCallback, this, 2)
	);
	menuButton->setPosition(fHalfWidth + fBtnWidth + fPadding, fBtnY);

	auto menu = Menu::create(resumeButton, restartButton, menuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	return true;
}

//菜单响应
void CPauseScene::OnMenuCallback(Ref* pSender, int iIndex)
{
	//重新开始按钮响应
	if (0 == iIndex)
	{
		Director::getInstance()->resume();
		Director::getInstance()->replaceScene(CGameScene::CreateScene());
	}

	//继续按钮响应
	if (1 == iIndex)
	{
		getParent()->removeFromParent();
		Director::getInstance()->resume();
		return;
	}

	//菜单按钮响应
	if (2 == iIndex)
	{
		Director::getInstance()->resume();
		Director::getInstance()->replaceScene(CThemeScene::CreateScene());
	}
}