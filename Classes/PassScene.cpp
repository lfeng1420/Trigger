#include "PassScene.h"
#include "DataManager.h"
#include "GameScene.h"
#include "ResManager.h"
#include "ThemeScene.h"

USING_NS_CC;

CPassScene::CPassScene()
{
}


CPassScene::~CPassScene()
{
}


//cocos function
Scene* CPassScene::CreateScene()
{
	auto layer = CPassScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
	
}

//cocos function
bool CPassScene::init()
{
	if (!LayerColor::initWithColor(Color4B(245, 222, 190, 255)))
	{
		return false;
	}

	//��ȡ��ǰ�ؿ�
	m_iCurTheme = UserDefault::getInstance()->getIntegerForKey("Theme");
	m_iCurLevel = UserDefault::getInstance()->getIntegerForKey("Level");

	m_visibleSize = Director::getInstance()->getVisibleSize();

	CreateBackground();

	ShowStar();

	CreateMenu();

	return true;
}


void CPassScene::CreateBackground()
{
	//auto bgSprite = Sprite::create("background.png");

	//level complete
	auto completeSprite = Sprite::create("images/levelcomplete.png");
	completeSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	addChild(completeSprite);
}


void CPassScene::ShowStar()
{
	float fScreenHeight = m_visibleSize.height;
	float fScreenWidth = m_visibleSize.width;

	//���Ǹ���
	int iStarNum = UserDefault::getInstance()->getIntegerForKey("Score");
	
	//������Ч
	char soundName[50] = { '\0' };
	sprintf(soundName, "sounds/sound_star%d", iStarNum);
	CResManager::getInstance()->PlayEffect(soundName);

	float fStarPadding = 0;
	float fCurX = 0;
	for (int i = 0; i < iStarNum; i++)
	{
		auto starSprite = Sprite::create("images/star.png");
		Size starSize = starSprite->getContentSize();
		//�����࣬������һ��
		if (fabs(fStarPadding) < 1e-6)
		{
			fStarPadding = (fScreenWidth - starSize.width * 3) / 4;	//Ĭ����3��
			//���ϵ�һ�μ��
			fCurX += fStarPadding;
		}

		//����λ��
		starSprite->setPosition(fCurX + starSize.width / 2, fScreenHeight * 0.7f);
		starSprite->setScale(2.0f);
		starSprite->setOpacity(0);
		fCurX += starSize.width + fStarPadding;
		addChild(starSprite);

		if (i > 0)
		{
			starSprite->runAction(
				Sequence::create(
					DelayTime::create(0.3f * i),
					Spawn::createWithTwoActions(
						ScaleTo::create(0.2f, 1.0f),
						FadeIn::create(0.2f)
					),
					NULL
				)
			);
		}
		else
		{
			starSprite->runAction(
				Spawn::createWithTwoActions(
					ScaleTo::create(0.2f, 1.0f),
					FadeIn::create(0.2f)
				)
			);
		}
		
	}
}


void CPassScene::CreateMenu()
{
	float fMenuPosY = m_visibleSize.height * 0.3f;
	float fHalfWidth = m_visibleSize.width * 0.5f;

	//���¿�ʼ��ť
	auto restartButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_finish_restart.png"),
		Sprite::createWithSpriteFrameName("btn_finish_restart.png"),
		CC_CALLBACK_1(CPassScene::OnMenuCallback, this, 0)
		);
	float fButtonWidth = restartButton->getContentSize().width;
	float fPadding = (m_visibleSize.width - 3 * fButtonWidth) / 4;
	restartButton->setPosition(fPadding + fButtonWidth / 2, fMenuPosY);

	//������ť
	auto resumeButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_finish_next.png"),
		Sprite::createWithSpriteFrameName("btn_finish_next.png"),
		CC_CALLBACK_1(CPassScene::OnMenuCallback, this, 1)
		);
	resumeButton->setPosition(fHalfWidth, fMenuPosY);

	//�˵���ť
	auto menuButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_finish_menu.png"),
		Sprite::createWithSpriteFrameName("btn_finish_menu.png"),
		CC_CALLBACK_1(CPassScene::OnMenuCallback, this, 2)
		);
	menuButton->setPosition(fHalfWidth + fButtonWidth + fPadding, fMenuPosY);

	auto menu = Menu::create(restartButton, resumeButton, menuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);
}


//�˵���Ӧ
void CPassScene::OnMenuCallback(Ref* pSender, int iIndex)
{
	//���¿�ʼ��ť��Ӧ
	if (0 == iIndex)
	{
		Director::getInstance()->replaceScene(CGameScene::CreateScene());
	}

	//��һ�ذ�ť��Ӧ
	if (1 == iIndex)
	{
		SetNextLevel();
		Director::getInstance()->replaceScene(CGameScene::CreateScene());
		return;
	}

	//�˵���ť��Ӧ
	if (2 == iIndex)
	{
		Director::getInstance()->replaceScene(CThemeScene::CreateScene());
	}
}


void CPassScene::SetNextLevel()
{
	//�ؿ��Ƿ����������һ��
	if (m_iCurLevel == LEVEL_INDEX_MAX)
	{
		if (m_iCurTheme == THEME_INDEX_MAX)
		{
			return;
		}

		m_iCurTheme += 1;
		m_iCurLevel = 0;
	}
	else
	{
		m_iCurLevel += 1;
	}

	UserDefault::getInstance()->setIntegerForKey("Theme", m_iCurTheme);
	UserDefault::getInstance()->setIntegerForKey("Level", m_iCurLevel);
}