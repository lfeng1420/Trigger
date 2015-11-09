#include "FailScene.h"
#include "GameScene.h"
#include "ThemeScene.h"

USING_NS_CC;

CFailScene::CFailScene()
{
}


CFailScene::~CFailScene()
{
}


//cocos function
Scene* CFailScene::CreateScene()
{
	auto layer = CFailScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
	
}

//cocos function
bool CFailScene::init()
{
	if (!LayerColor::initWithColor(Color4B(245, 222, 190, 255)))
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	float fScrWidth = visibleSize.width;
	float fScrHeight = visibleSize.height;

	auto bgSprite = Sprite::create("images/nomorebeat.png");
	bgSprite->setPosition(fScrWidth / 2, fScrHeight * 0.6f);
	addChild(bgSprite);


	//���¿�ʼ
	auto restartButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_finish_restart.png"),
		Sprite::createWithSpriteFrameName("btn_finish_restart.png"),
		CC_CALLBACK_1(CFailScene::OnMenuCallback, this, 0)
		);
	Size fBtnSize = restartButton->getContentSize();
	float fPadding = (fScrWidth - 2 * fBtnSize.width) / 3;
	//�˵���Y��λ�ã���Ļһ��߶ȼ�ȥ2��NO MORE BEAT���ָ߶�
	float fBtnY = fScrHeight * 0.4f;
	restartButton->setPosition(fPadding + fBtnSize.width / 2, fBtnY);

	//�˵���ť
	auto menuButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_finish_menu.png"),
		Sprite::createWithSpriteFrameName("btn_finish_menu.png"),
		CC_CALLBACK_1(CFailScene::OnMenuCallback, this, 1)
		);
	menuButton->setPosition(fScrWidth - fPadding - fBtnSize.width / 2, fBtnY);

	auto menu = Menu::create(restartButton, menuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	return true;
}


//�˵���Ӧ
void CFailScene::OnMenuCallback(cocos2d::Ref* pSender, int iIndex)
{
	//���¿�ʼ��ť��Ӧ
	if (0 == iIndex)
	{
		Director::getInstance()->replaceScene(CGameScene::CreateScene());
	}

	//�˵���ť��Ӧ
	if (1 == iIndex)
	{
		Director::getInstance()->replaceScene(CThemeScene::CreateScene());
	}
}