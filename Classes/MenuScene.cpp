#include "MenuScene.h"
#include "DataManager.h"
#include "ResManager.h"
#include <2d/CCActionInterval.h>
#include "GameScene.h"
#include "ThemeScene.h"
#include "ThankScene.h"

USING_NS_CC;

CMenuScene::CMenuScene()
{
}


CMenuScene::~CMenuScene()
{
}


Scene* CMenuScene::CreateScene()
{
	auto layer = CMenuScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
}


bool CMenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size m_visibleSize = Director::getInstance()->getVisibleSize();

	float fScreenWidth = m_visibleSize.width;
	float fScreenHeight = m_visibleSize.height;

	//����ִ��ʱ��
	float fActionTime = 0.4f;

	//������ʼλ��
	Vec2 actionStartPos = Vec2(fScreenWidth * 1.5f, fScreenHeight / 2);
	float fDelayTime = 0;

	//����
	auto bgSprite = Sprite::create("images/background.png");
	bgSprite->setPosition(Vec2(fScreenWidth / 2, fScreenHeight / 2));
	addChild(bgSprite);

	//��Ϸ����Logo
	auto logoSprite = Sprite::create("images/title.png");
	logoSprite->setPosition(Vec2(fScreenWidth / 2, fScreenHeight * 1.5f));
	addChild(logoSprite);
	auto ActionForLogo = Sequence::create(
		MoveTo::create(fActionTime, Vec2(fScreenWidth / 2, fScreenHeight * 0.75f) - Vec2(0, ACTION_REBOUND_LEN)),
		CallFuncN::create(CC_CALLBACK_1(CMenuScene::OnActionFinish, this, false, false)),
		NULL
		);
	fDelayTime += fActionTime / 2;

	//����Sprite
	auto scoreSprite = Sprite::create("images/star.png");
	Size scoreSize = scoreSprite->getContentSize();

	//��ȡ��ǰ����
	int iCurScore = CDataManager::getInstance()->GetTotalScore();
	char score[10] = {'\0'};
	sprintf(score, "%d", iCurScore);

	//������ʼλ��
	float fCurX = scoreSize.width + 10;
	for (int i = 0; i < strlen(score); i++)
	{
		//ͼƬ����
		char name[20] = {'\0'};
		sprintf(name, "number_b_%d.png", score[i] - '0');

		//����Sprite
		auto numSprite = Sprite::createWithSpriteFrameName(name);
		float fNumWidth = numSprite->getContentSize().width;
		float fNumHeight = numSprite->getContentSize().height;

		numSprite->setPosition(fCurX + fNumWidth / 2, fNumHeight / 2 + 5);
		scoreSprite->addChild(numSprite);

		fCurX += fNumWidth;
		if (i < strlen(score) - 1)
		{
			fCurX += 2;
		}
	}

	Vec2 scorePos = Vec2(fScreenWidth, fScreenHeight * 0.55f);
	scoreSprite->setPosition(scorePos + Vec2(fScreenWidth / 2, 0));
	addChild(scoreSprite);
	scorePos.x = (scorePos.x - fCurX) / 2 + scoreSize.width / 2 - ACTION_REBOUND_LEN;
	auto ActionForScore = Sequence::create(
		DelayTime::create(fDelayTime),
		MoveTo::create(fActionTime, scorePos),
		CallFuncN::create(CC_CALLBACK_1(CMenuScene::OnActionFinish, this, true, false)),
		NULL
		);
	fDelayTime += fActionTime / 2;

	//��ʼ��ť
	auto playButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_start.png"),
		Sprite::createWithSpriteFrameName("btn_start.png"),
		CC_CALLBACK_1(CMenuScene::OnButtonClick, this, 0)
		);
	playButton->setPosition(Vec2(fScreenWidth * 1.5f, fScreenHeight * 0.35f));
	//��ʼ��ť����
	auto ActionForPlayBtn = Sequence::create(
		DelayTime::create(fDelayTime),
		MoveTo::create(fActionTime, Vec2(fScreenWidth * 0.5f, fScreenHeight * 0.35f) - Vec2(ACTION_REBOUND_LEN, 0)),
		CallFuncN::create(CC_CALLBACK_1(CMenuScene::OnActionFinish, this, true, false)),
		NULL
		);
	fDelayTime += fActionTime / 2;

	Vec2 buttonPos = Vec2(fScreenWidth * 0.5f, fScreenHeight * 0.16f);
	//������ť
	auto helpButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_help.png"),
		Sprite::createWithSpriteFrameName("btn_help.png"),
		CC_CALLBACK_1(CMenuScene::OnButtonClick, this, 1)
		);
	//ÿ����ť��ࣺ�趨ÿ����ť���һ��
	float fPadding = (fScreenWidth - helpButton->getContentSize().width * 4) / 5.0f / 2.0f;
	Vec2 helpPos = Vec2(buttonPos.x - fPadding * 3 - helpButton->getContentSize().width * 3 / 2, buttonPos.y);
	helpButton->setPosition(helpPos.x, -helpPos.y);
	//������ť����
	auto ActionForHelpBtn = Sequence::create(
		DelayTime::create(fDelayTime),
		MoveTo::create(fActionTime, Vec2(helpPos.x, helpPos.y + ACTION_REBOUND_LEN)),
		CallFuncN::create(CC_CALLBACK_1(CMenuScene::OnActionFinish, this, false, true)),
		NULL
		);
	fDelayTime += fActionTime / 2;

	//���ఴť
	auto moreButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_more.png"),
		Sprite::createWithSpriteFrameName("btn_more.png"),
		CC_CALLBACK_1(CMenuScene::OnButtonClick, this, 2)
		);
	Vec2 morePos = Vec2(buttonPos.x - fPadding - moreButton->getContentSize().width / 2, buttonPos.y);
	moreButton->setPosition(morePos.x, -morePos.y);
	//���ఴť����
	auto ActionForMoreBtn = Sequence::create(
		DelayTime::create(fDelayTime),
		MoveTo::create(fActionTime, Vec2(morePos.x, morePos.y + ACTION_REBOUND_LEN)),
		CallFuncN::create(CC_CALLBACK_1(CMenuScene::OnActionFinish, this, false, true)),
		NULL
		);
	fDelayTime += fActionTime / 2;

	//���ǰ�ť
	auto starButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_star.png"),
		Sprite::createWithSpriteFrameName("btn_star.png"),
		CC_CALLBACK_1(CMenuScene::OnButtonClick, this, 3)
		);
	Vec2 starPos = Vec2(buttonPos.x + fPadding + starButton->getContentSize().width / 2, buttonPos.y);
	starButton->setPosition(starPos.x, -starPos.y);
	//���ǰ�ť����
	auto ActionForStarBtn = Sequence::create(
		DelayTime::create(fDelayTime),
		MoveTo::create(fActionTime, Vec2(starPos.x, starPos.y + ACTION_REBOUND_LEN)),
		CallFuncN::create(CC_CALLBACK_1(CMenuScene::OnActionFinish, this, false, true)),
		NULL
		);
	fDelayTime += fActionTime / 2;

	//�������ذ�ť
	bool bSoundState = CResManager::getInstance()->GetSoundState();
	const char* spriteName = bSoundState ? "btn_sound_on.png" : "btn_sound_off.png";
	auto soundButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName(spriteName),
		Sprite::createWithSpriteFrameName(spriteName),
		CC_CALLBACK_1(CMenuScene::OnButtonClick, this, 4)
		);
	Vec2 soundPos = Vec2(buttonPos.x + fPadding * 3 + soundButton->getContentSize().width * 3 / 2, buttonPos.y);
	soundButton->setPosition(soundPos.x, -soundPos.y);
	//������ť����
	auto ActionForSoundBtn = Sequence::create(
		DelayTime::create(fDelayTime),
		MoveTo::create(fActionTime, Vec2(soundPos.x, soundPos.y + ACTION_REBOUND_LEN)),
		CallFuncN::create(CC_CALLBACK_1(CMenuScene::OnActionFinish, this, false, true)),
		NULL
		);
	fDelayTime += fActionTime / 2;
	
	auto menu = Menu::create(playButton, helpButton, moreButton, starButton, soundButton, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	//���ö���
	logoSprite->runAction(ActionForLogo);
	scoreSprite->runAction(ActionForScore);
	playButton->runAction(ActionForPlayBtn);
	helpButton->runAction(ActionForHelpBtn);
	moreButton->runAction(ActionForMoreBtn);
	starButton->runAction(ActionForStarBtn);
	soundButton->runAction(ActionForSoundBtn);

	//��������
	CreateKeyListener();

	return true;
}


//�����ť����Ӧ
void CMenuScene::OnButtonClick(Ref* pSender, int iIndex)
{
	if (0 == iIndex)
	{
		log("StartButton");

		//�����¼
		UserDefault::getInstance()->setIntegerForKey("Theme", -1);
		UserDefault::getInstance()->setIntegerForKey("Level", 0);

		//�л�����
		auto scene = CThemeScene::CreateScene();
		Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3f, scene));
		return;
	}

	if (1 == iIndex)
	{
		log("helpButton");
		auto scene = CThankScene::CreateScene();
		Director::getInstance()->replaceScene(scene);
		return;
	}

	if (2 == iIndex)
	{
		log("moreButton");
		auto scene = CThankScene::CreateScene();
		Director::getInstance()->replaceScene(scene);
		return;
	}
	
	if (3 == iIndex)
	{
		log("starButton");
		auto scene = CThankScene::CreateScene();
		Director::getInstance()->replaceScene(scene);
		return;
	}

	if (4 == iIndex)
	{
		log("soundButton");
		MenuItemSprite* sprite = (MenuItemSprite*)pSender;
		//����״̬
		bool bSoundState = !(CResManager::getInstance()->GetSoundState());
		const char* spriteName = bSoundState ? "btn_sound_on.png" : "btn_sound_off.png";
		sprite->setNormalImage(Sprite::createWithSpriteFrameName(spriteName));
		sprite->setSelectedImage(Sprite::createWithSpriteFrameName(spriteName));
		CResManager::getInstance()->SetSoundState(bSoundState);
		return;
	}
}


//����ִ�н�����Ļص������ڻص�Ч��
//bChangeXFlag��ʾ�����Ƿ�ı�X��
//bIncreaseFlag��ʾ�����ı�ĳһ��ʱ�Ƿ���������
void CMenuScene::OnActionFinish(Ref* pSender, bool bChangeXFlag, bool bIncreaseFlag)
{
	Sprite* pSprite = (Sprite*)pSender;
	Vec2 curPos = pSprite->getPosition();
	if (bChangeXFlag)
	{
		curPos.x += ACTION_REBOUND_LEN * (bIncreaseFlag ? -1 : 1);
	}
	else
	{
		curPos.y += ACTION_REBOUND_LEN * (bIncreaseFlag ? -1 : 1);
	}
	
	pSprite->runAction(MoveTo::create(0.2f, curPos));
}


//������������������Ҫ�������ؼ�
void CMenuScene::CreateKeyListener()
{
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (EventKeyboard::KeyCode::KEY_RETURN == keyCode ||
			EventKeyboard::KeyCode::KEY_ESCAPE == keyCode ||
			EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode)
		{
			//��������
			CDataManager::getInstance()->SaveUserData();
			//��������״̬
			CResManager::getInstance()->SaveSoundState();

			Director::getInstance()->end();
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
}
