#include "ThankScene.h"
#include "MenuScene.h"

USING_NS_CC;

CThankScene::CThankScene()
{
}


CThankScene::~CThankScene()
{
}


//cocos function
Scene* CThankScene::CreateScene()
{
	auto layer = CThankScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
}


//cocos function
bool CThankScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_visibleSize = Director::getInstance()->getVisibleSize();

	//背景
	auto bgSprite = Sprite::create("images/background.png");
	bgSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	addChild(bgSprite);

	//Logo
	m_pLogoSprite = Sprite::create("images/title.png");
	m_pLogoSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height * 0.75f - 20);
	m_pLogoSprite->runAction(
		MoveBy::create(0.4f, Vec2(0, -(m_visibleSize.height * 0.1f - 20)))
	);
	addChild(m_pLogoSprite);

	//说明，随机选择
	int iRand = CCRANDOM_0_1() * 10000;
	bool bZhFlag = (iRand >= 5000);
	m_pInfoSprite = Sprite::create(bZhFlag ? "images/zh_thanks.png" : "images/en_thanks.png");
	m_pInfoSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height * 0.4f);
	m_pInfoSprite->setOpacity(0);
	m_pInfoSprite->runAction(
		Sequence::createWithTwoActions(
			DelayTime::create(0.4f),
			Spawn::createWithTwoActions(
				FadeIn::create(0.5f),
				MoveBy::create(0.5f, Vec2(0, m_visibleSize.height * 0.1f))
			)
		)
	);
	addChild(m_pInfoSprite);

	CreateKeyListener();

	CreateTouchListener();

	return true;
}


//创建按键监听器，主要监听返回键
void CThankScene::CreateKeyListener()
{
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (EventKeyboard::KeyCode::KEY_RETURN == keyCode ||
			EventKeyboard::KeyCode::KEY_ESCAPE == keyCode ||
			EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode)
		{
			TakeAction();
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}


void CThankScene::CreateTouchListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		return true;
	};

	touchListener->onTouchMoved = [&](Touch* touch, Event* event)
	{

	};

	touchListener->onTouchEnded = [&](Touch* touch, Event* event)
	{
		TakeAction();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}


//执行动作
void CThankScene::TakeAction()
{
	//Logo往上淡出
	m_pLogoSprite->runAction(
		Sequence::create(
			Spawn::createWithTwoActions(
				MoveBy::create(0.25f, Vec2(0, m_visibleSize.height * 0.5f)),
				FadeOut::create(0.25f)
			),
			CallFuncN::create(CC_CALLBACK_1(CThankScene::ReturnMenu, this)),
			NULL
		)
	);

	//信息往下淡出
	m_pInfoSprite->runAction(
		Spawn::createWithTwoActions(
				MoveBy::create(0.25f, Vec2(0, -m_visibleSize.height * 0.5f)),
				FadeOut::create(0.25f)
		)
	);
}


//返回菜单界面
void CThankScene::ReturnMenu(Node* pSender)
{
	auto scene = CMenuScene::CreateScene();
	Director::getInstance()->replaceScene(/*TransitionSlideInB::create(0.3f, */scene/*)*/);
}