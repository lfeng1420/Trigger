#include "ThemeScene.h"
#include "DataManager.h"
#include "ResManager.h"
#include "MenuScene.h"
#include "GameScene.h"

USING_NS_CC;




CThemeScene::CThemeScene()
{
}


CThemeScene::~CThemeScene()
{
}


//cocos function
Scene* CThemeScene::CreateScene()
{
	auto layer = CThemeScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
}


//cocos function
bool CThemeScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_visibleSize = Director::getInstance()->getVisibleSize();

	InitBGAndBtn();

	InitScrollView();

	CreateTouchListener();

	CreateKeyListener();

	return true;
}


//初始化背景
void CThemeScene::InitBGAndBtn()
{
	//背景
	m_pBgSprite = Sprite::create("images/background.png");
	m_pBgSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	addChild(m_pBgSprite);

	//返回按钮
	m_pBackBtn = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_back.png"),
		Sprite::createWithSpriteFrameName("btn_back.png"),
		CC_CALLBACK_1(CThemeScene::OnMenuClick, this, BtnIndex_Back)
	);
	Size backBtnSize = m_pBackBtn->getContentSize();
	m_pBackBtn->setPosition(backBtnSize.height, m_visibleSize.height - backBtnSize.height);
	
	//菜单
	auto menu = Menu::create(m_pBackBtn, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	//关卡指针Sprite
	auto pointSprite = Sprite::create("images/missionchoose.png");
	Size pointSize = pointSprite->getContentSize();
	pointSprite->setPosition(pointSize.width / 2, m_visibleSize.height / 2);
	m_pointBoundingBox = pointSprite->getBoundingBox();
	addChild(pointSprite);
}


//设置背景
void CThemeScene::SetBackground()
{
	char arrSpriteName[20] = { '\0' };
	sprintf(arrSpriteName, "%d.png", m_iSelectTheme);
	//m_pBgSprite->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(arrSpriteName));
}


//初始化Layer
void CThemeScene::InitThemeLayer()
{
	float fScreenHeight = m_visibleSize.height;
	float fScreenWidth = m_visibleSize.width;

	//创建层
	m_pThemeLayer = Layer::create();
	m_pThemeLayer->setPosition(fScreenWidth / 2, fScreenHeight / 2);

	//获取未通过的关卡
	int iUnpassLevel = CDataManager::getInstance()->GetUnpassLevel();
	int iUnpassTheme = iUnpassLevel / Level_Num;

	char arrSpriteName[20] = { '\0' };

	//重置位置
	float fCurX = fScreenWidth * 0.5f;
	float fCurY = fScreenHeight;
	for (int i = 0; i < Theme_Num; i++)
	{
		sprintf(arrSpriteName, "%d.png", i);
		auto sprite = Sprite::createWithSpriteFrameName(arrSpriteName);
		m_themeSize = sprite->getContentSize();
		sprite->setPosition(fCurX, fCurY - m_themeSize.height / 2 - 1);
		
		if (i <= iUnpassTheme)
		{
			int iThemeScore = CDataManager::getInstance()->GetThemeScore(i);
			//星星
			auto starSprite = Sprite::create("images/star_w.png");
			Size starSize = starSprite->getContentSize();

			//主题当前分数十位
			char arrSpriteName[20] = { '\0' };
			sprintf(arrSpriteName, "number_s_%d.png", iThemeScore /10);
			auto curScoreDecadeSprite = Sprite::createWithSpriteFrameName(arrSpriteName);
			Size numSize = curScoreDecadeSprite->getContentSize();

			//计算间距
			float fPadding = (fScreenWidth - starSize.width - 20 - (numSize.width + 2) * 5) / 2;
			
			//主题当前分数个位
			sprintf(arrSpriteName, "number_s_%d.png", iThemeScore % 10);
			auto curScoreUnitSprite = Sprite::createWithSpriteFrameName(arrSpriteName);

			//间隔符
			auto slashSprite = Sprite::createWithSpriteFrameName("number_s_10.png");

			//总分数：60
			auto totalScoreDecadeSprite = Sprite::createWithSpriteFrameName("number_s_6.png");
			auto totalScoreUnitSprite = Sprite::createWithSpriteFrameName("number_s_0.png");

			//设置位置
			float fCurX = fPadding;
			float fCurY = m_themeSize.height / 2;

			starSprite->setPosition(fCurX + starSize.width / 2, fCurY);
			sprite->addChild(starSprite);
			fCurX += starSize.width + 20;

			curScoreDecadeSprite->setPosition(fCurX + numSize.width / 2, fCurY);
			sprite->addChild(curScoreDecadeSprite);
			fCurX += numSize.width + 2;

			curScoreUnitSprite->setPosition(fCurX + numSize.width / 2, fCurY);
			sprite->addChild(curScoreUnitSprite);
			fCurX += numSize.width + 2;

			slashSprite->setPosition(fCurX + numSize.width / 2, fCurY);
			sprite->addChild(slashSprite);
			fCurX += numSize.width + 2;

			totalScoreDecadeSprite->setPosition(fCurX + numSize.width / 2, fCurY);
			sprite->addChild(totalScoreDecadeSprite);
			fCurX += numSize.width + 2;

			totalScoreUnitSprite->setPosition(fCurX + numSize.width / 2, fCurY);
			sprite->addChild(totalScoreUnitSprite);
		}
		else
		{
			//lock
			auto lockSprite = Sprite::createWithSpriteFrameName("lock_w_big.png");
			lockSprite->setPosition(m_themeSize.width / 2, m_themeSize.height / 2);
			sprite->addChild(lockSprite);
		}

		m_pThemeLayer->addChild(sprite);
		m_themeSprVec.pushBack(sprite);

		//下一个位置
		fCurY -= m_themeSize.height - 1;
	}
}


//初始化关卡Layer
void CThemeScene::InitLevelLayer()
{
	m_pLevelLayer = Layer::create();
	m_pLevelLayer->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);

	//关卡
	Size numSize = m_oLevelSpr[0].GetNumSize();
	Size starSize = m_oLevelSpr[0].GetStarSize();
	float fCurY = m_visibleSize.height;
	for (int i = 0; i < Level_Num; i++)
	{
		auto levelNode = Node::create();
		levelNode->setContentSize(Size(m_visibleSize.width, Level_Height));

		//初始化
		m_oLevelSpr[i].SetLock(0);
		m_oLevelSpr[i].SetDec(0, (i + 1) / 10);
		m_oLevelSpr[i].SetUni(0, (i + 1) % 10);

		//设置位置
		m_oLevelSpr[i].SetPos((i + 1) >= 10);

		levelNode->addChild(m_oLevelSpr[i].m_pNumDecSpr);
		levelNode->addChild(m_oLevelSpr[i].m_pNumUniSpr);
		levelNode->addChild(m_oLevelSpr[i].m_pLockSpr);
		for (int j = 0; j < 3; j++)
		{
			levelNode->addChild(m_oLevelSpr[i].m_pStarSpr[j]);
		}

		levelNode->setPosition(0, fCurY);
		m_pLevelLayer->addChild(levelNode);
		fCurY -= Level_Height;
	}
}


//初始化ScrollView
void CThemeScene::InitScrollView()
{
	//------------关卡ScrollView-------------
	m_pLevelScrollView = ScrollView::create();
	//创建层
	InitLevelLayer();
	m_pLevelScrollView->setContainer(m_pLevelLayer);
	//内容大小
	m_pLevelScrollView->setContentSize(Size(m_visibleSize.width, Level_Height * Level_Num));

	//设置位置
	m_pLevelScrollView->setPosition(Vec2::ZERO);

	//设置可视大小
	m_pLevelScrollView->setViewSize(m_visibleSize);

	//设置偏移
	float fMinOffset = -m_visibleSize.height / 2 - Level_Height / 2;
	m_pLevelScrollView->setContentOffset(Vec2(0, fMinOffset));

	//设置不可触摸
	m_pLevelScrollView->setTouchEnabled(false);

	//设置方向
	m_pLevelScrollView->setDirection(ScrollView::Direction::VERTICAL);

	//设置委托
	m_pLevelScrollView->setDelegate(this);
	m_pLevelScrollView->unscheduleAllSelectors();
	addChild(m_pLevelScrollView);

	//------------主题ScrollView-------------
	m_pThemeScrollView = ScrollView::create();

	//初始化ScrollView包含的层
	InitThemeLayer();
	m_pThemeScrollView->setContainer(m_pThemeLayer);

	//设置位置
	m_pThemeScrollView->setPosition(Vec2::ZERO);

	//设置可视大小
	m_pThemeScrollView->setViewSize(m_visibleSize);

	//设置偏移
	m_pThemeScrollView->setContentOffset(Vec2::ZERO);

	//设置所有内容大小
	m_pThemeScrollView->setContentSize(Size(m_visibleSize.width, Theme_Height * Theme_Num));

	//设置不可触摸
	m_pThemeScrollView->setTouchEnabled(false);

	//设置方向
	m_pThemeScrollView->setDirection(ScrollView::Direction::VERTICAL);

	//设置委托
	m_pThemeScrollView->setDelegate(this);
	m_pThemeScrollView->unscheduleAllSelectors();
	addChild(m_pThemeScrollView);

	//默认显示主题选择界面
	m_pLevelScrollView->setVisible(false);
	m_pThemeScrollView->setVisible(true);

	m_iSelectTheme = UserDefault::getInstance()->getIntegerForKey("Theme", -1);
	int iLevel = UserDefault::getInstance()->getIntegerForKey("Level", 0);
	log("iTheme=%d, iLevel=%d", m_iSelectTheme, iLevel);
	if (m_iSelectTheme < 0)
	{
		//默认显示主题选择界面
		m_pLevelScrollView->setVisible(false);
		m_pThemeScrollView->setVisible(true);
	}
	else
	{
		//构建关卡列表
		BuildLevel();

		//默认显示主题选择界面
		m_pLevelScrollView->setVisible(true);
		m_pThemeScrollView->setVisible(false);

		//主题列表提前设置上下移出动作结束
		for (int i = 0; i < m_themeSprVec.size(); i++)
		{
			Sprite* pSprite = m_themeSprVec.at(i);
			Vec2 pos = pSprite->getPosition();
			pos += Vec2(0, i > m_iSelectTheme ? -m_visibleSize.height : m_visibleSize.height);
			pSprite->setPosition(pos);
		}

		//设置关卡列表偏移
		float fOffset = iLevel * Level_Height + fMinOffset;
		m_pLevelScrollView->setContentOffset(Vec2(0, fOffset));
	}
}


void CThemeScene::BuildLevel()
{
	//默认的下一关
	int iPassLevel = -1;

	//记录分数
	int arrScore[20] = { 0 };

	//检查当前未通过的关卡
	int iUnpassLevel = CDataManager::getInstance()->GetUnpassLevel();
	int iRunningTheme = iUnpassLevel / Level_Num;
	log("BuildLevel: m_iSelectTheme = %d iUnpassLevel=%d", m_iSelectTheme, iUnpassLevel);
	if (m_iSelectTheme < iRunningTheme)
	{
		iPassLevel = Level_Num - 1;
	}
	else if (m_iSelectTheme == iRunningTheme)
	{
		iPassLevel = iUnpassLevel - iUnpassLevel / Level_Num * Level_Num;
	}

	//解锁的部分需要显示关卡数字
	float fCurX = m_visibleSize.width / 2;
	float fCurY = m_visibleSize.height;
	for (int i = 0; i <= iPassLevel; i++)
	{
		m_oLevelSpr[i].SetLockState(false);
		m_oLevelSpr[i].SetDec(m_iSelectTheme, (i + 1) / 10);
		m_oLevelSpr[i].SetUni(m_iSelectTheme, (i + 1) % 10);
		//获取分数
		int iScore = CDataManager::getInstance()->GetLevelScore(m_iSelectTheme, i);
		m_oLevelSpr[i].SetStar(iScore);
	}

	//剩下的是锁住的关卡
	for (int i = iPassLevel + 1; i < Level_Num; i++)
	{
		m_oLevelSpr[i].SetLock(m_iSelectTheme);
		m_oLevelSpr[i].SetLockState(true);
	}
}


//创建触摸监听器
void CThemeScene::CreateTouchListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		if (m_iSelectTheme < 0)
		{
			m_themeTouchPos = touch->getLocation();
			m_themeOffsetPos = m_pThemeScrollView->getContentOffset();
		}
		else
		{
			m_levelTouchPos = touch->getLocation();
			m_levelOffsetPos = m_pLevelScrollView->getContentOffset();
		}
		return true;
	};

	touchListener->onTouchMoved = [&](Touch* touch, Event* event)
	{
		if (m_iSelectTheme < 0)
		{
			Vec2 destPos = touch->getLocation();
			float fCurY = destPos.y - m_themeTouchPos.y + m_themeOffsetPos.y;
			if (fCurY < 0)
			{
				fCurY = 0;
			}

			float fExtraHeight = m_pThemeScrollView->getContentSize().height - m_visibleSize.height;
			if (fCurY > fExtraHeight)
			{
				fCurY = fExtraHeight;
			}
			m_pThemeScrollView->setContentOffset(Vec2(0, fCurY));
		}
		else
		{
			Vec2 destPos = touch->getLocation();
			float fCurY = destPos.y - m_levelTouchPos.y + m_levelOffsetPos.y;
			float fHalfHeight = m_visibleSize.height / 2;
			float fMaxOffset = Level_Height * (Level_Num - 1) - fHalfHeight - Level_Height / 2;
			float fMinOffset = -fHalfHeight - Level_Height / 2;
			if (fCurY < fMinOffset)
			{
				fCurY = fMinOffset;
			}

			if (fCurY > fMaxOffset)
			{
				fCurY = fMaxOffset;
			}

			m_pLevelScrollView->setContentOffset(Vec2(0, fCurY));
			Vec2 offSetPos = m_pLevelScrollView->getContentOffset();
		}
	};
	touchListener->onTouchEnded = [&](Touch* touch, Event* event)
	{
		if (m_iSelectTheme < 0)
		{
			Vec2 destPos = touch->getLocation();
			Vec2 offsetPos = m_pThemeScrollView->getContentOffset();
			float fSlideY = (destPos.y + offsetPos.y) - (m_themeTouchPos.y + m_themeOffsetPos.y);
			//滑动距离小于20算作点击
			if (fabs(fSlideY) < 20)
			{
				//滑动高度
				float fScreenHeight = m_visibleSize.height;
				float fSlideHeight = m_themeOffsetPos.y + fScreenHeight - m_themeTouchPos.y;
				//获取点击的主题
				int iTouchTheme = fSlideHeight / (m_themeSize.height - 1);
				//获取当前等待完全通关的主题
				int iUnpassLevel = CDataManager::getInstance()->GetUnpassLevel();
				if (iTouchTheme <= iUnpassLevel / 20)
				{
					//已解锁的主题才可以打开
					OnMenuClick(nullptr, iTouchTheme);
				}

				return;
			}

			//检查是不是要滑回菜单界面
			log("%f %f %f", destPos.y, m_themeTouchPos.y, m_themeOffsetPos.y);
			if (destPos.y - m_themeTouchPos.y + m_themeOffsetPos.y < -100)
			{
				//返回菜单界面
				auto scene = CMenuScene::CreateScene();
				Director::getInstance()->replaceScene(TransitionSlideInT::create(0.3f, scene));
			}
		}
		else
		{
			Vec2 destPos = touch->getLocation();
			Vec2 offsetPos = m_pLevelScrollView->getContentOffset();
			
			//计算滑动到当前关卡的位置
			float fHalfHeight = m_visibleSize.height / 2;
			float fMinOffset = -fHalfHeight - Level_Height / 2;
			float fCurOffset = offsetPos.y - fMinOffset;

			//当前关卡
			int iLevel = ((int)fCurOffset) / Level_Height;
			if (fCurOffset > (iLevel + 0.5f) * Level_Height)
			{
				iLevel += 1;
			}
			log("iLevel = %d", iLevel);

			//调整偏移
			m_pLevelScrollView->setContentOffset(Vec2(0, iLevel * Level_Height + fMinOffset));

			//滑动距离小于20算作点击，解锁的情况下才可以进入游戏
			bool bLockState = m_oLevelSpr[iLevel].GetLockState();
			bool bClickPoint = m_pointBoundingBox.containsPoint(destPos);
			if (fabs(offsetPos.y - m_levelOffsetPos.y) < 20 && !bLockState && bClickPoint)
			{
				//记录当前主题和关卡
				UserDefault::getInstance()->setIntegerForKey("Theme", m_iSelectTheme);
				UserDefault::getInstance()->setIntegerForKey("Level", iLevel);

				//设置需要执行动作
				UserDefault::getInstance()->setBoolForKey("HexagonAction", true);

				//切换至游戏界面
				auto scene = CGameScene::CreateScene();
				Director::getInstance()->replaceScene(scene);
			}
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}


//点击菜单项
void CThemeScene::OnMenuClick(Ref* pSender, int iIndex)
{
	log("OnMenuClick %d", iIndex);
	if (iIndex < BtnIndex_Back)
	{
		m_iSelectTheme = iIndex;
		//根据选择的主题设置背景
		//SetBackground();

		//构建关卡列表
		BuildLevel();

		//主题列表执行动作，从选择的主题开始分开往上/下移动
		TakeThemeAction(true);
	}
	else if (iIndex == BtnIndex_Back)
	{
		//主题列表重新聚合
		TakeThemeAction(false);

		//重置选择的主题
		m_iSelectTheme = -1;
	}
}


//主题Sprite执行动作，bMoveOut表示是否移出
void CThemeScene::TakeThemeAction(bool bMoveOut)
{
	//执行动作前需要设置为不隐藏
	m_pThemeScrollView->setVisible(true);
	//要显示时需要提前显示关卡列表和返回按钮
	if (bMoveOut)
	{
		m_pBackBtn->setVisible(true);
		m_pLevelScrollView->setVisible(true);
	}

	float fScreenHeight = m_visibleSize.height;
	for (int i = 0; i <= m_iSelectTheme; i++)
	{
		Sprite* pSprite = m_themeSprVec.at(i);
		pSprite->runAction(
			Sequence::create(
				MoveBy::create(0.5f, Vec2(0, bMoveOut ? fScreenHeight : -fScreenHeight)),
				CallFuncN::create(CC_CALLBACK_1(CThemeScene::OnThemeActionCallback, this, bMoveOut)),
				NULL
			)
		);
	}

	for (int i = m_iSelectTheme + 1; i < m_themeSprVec.size(); i++)
	{
		Sprite* pSprite = m_themeSprVec.at(i);
		pSprite->runAction(
			MoveBy::create(0.4f, Vec2(0, (bMoveOut ? -fScreenHeight : fScreenHeight)))
		);
	}
}


//Theme动作回调
void CThemeScene::OnThemeActionCallback(Node* pSender, bool bMoveOut)
{
	m_pThemeScrollView->setVisible(!bMoveOut);
	m_pLevelScrollView->setVisible(bMoveOut);
	//如果回到主题列表，则隐藏返回按钮，重置LevelScrollView偏移
	if (!bMoveOut)
	{
		m_pBackBtn->setVisible(false);
		//设置偏移
		float fHalfHeight = m_visibleSize.height / 2;
		m_pLevelScrollView->setContentOffset(Vec2(0, -fHalfHeight - Level_Height / 2));
	}
}


//创建按键监听器，主要监听返回键
void CThemeScene::CreateKeyListener()
{
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (EventKeyboard::KeyCode::KEY_RETURN == keyCode ||
			EventKeyboard::KeyCode::KEY_ESCAPE == keyCode ||
			EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode)
		{
			//只在主题选择界面才可以返回
			if (m_iSelectTheme < 0)
			{
				//返回菜单界面
				auto scene = CMenuScene::CreateScene();
				Director::getInstance()->replaceScene(TransitionSlideInT::create(0.3f, scene));
			}
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}


LevelSpr::LevelSpr()
{
	m_pNumDecSpr = Sprite::create();
	m_pNumUniSpr = Sprite::create();
	m_pLockSpr = Sprite::create();
	for (int i = 0; i < 3; i++)
	{
		m_pStarSpr[i] = Sprite::create("images/star_w.png");
	}
}

void LevelSpr::SetDec(int iTheme, int iNum)
{
	if (iNum == 0)
	{
		m_pNumDecSpr->setVisible(false);
	}
	char arrSpriteName[20] = { '\0' };
	sprintf(arrSpriteName, "number_%d_%d.png", iTheme, iNum);
	m_pNumDecSpr->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(arrSpriteName));
}

void LevelSpr::SetUni(int iTheme, int iNum)
{
	char arrSpriteName[20] = { '\0' };
	sprintf(arrSpriteName, "number_%d_%d.png", iTheme, iNum);
	m_pNumUniSpr->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(arrSpriteName));
}

void LevelSpr::SetLock(int iTheme)
{
	char arrSpriteName[20] = { '\0' };
	sprintf(arrSpriteName, "lock_%d_big.png", iTheme);
	m_pLockSpr->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(arrSpriteName));
}

void LevelSpr::SetLockState(bool bLock)
{
	//锁
	m_pLockSpr->setVisible(bLock);
	//数字
	m_pNumDecSpr->setVisible(!bLock);
	m_pNumUniSpr->setVisible(!bLock);
	
	//如果要显示锁，那么隐藏所有星星
	if (bLock)
	{
		SetStar(0);
	}

	m_bLockState = bLock;
}


Size LevelSpr::GetNumSize()
{
	return m_pNumDecSpr->getContentSize();
}


Size LevelSpr::GetStarSize()
{
	return m_pStarSpr[0]->getContentSize();
}


void LevelSpr::SetPos(bool bDoubleNum)
{

	Size numSize = GetNumSize();
	Size starSize = GetStarSize();
	float fPadding = 4;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//数字位置：区分两位和一位
	float fCurX = visibleSize.width / 2;
	float fCurY = CThemeScene::Level_Height / 2;
	if (bDoubleNum)
	{
		m_pNumDecSpr->setPosition(fCurX - fPadding / 2 - numSize.width / 2, fCurY);
		m_pNumUniSpr->setPosition(fCurX + fPadding / 2 + numSize.width / 2, fCurY);
		fCurX += fPadding / 2;
	}
	else
	{
		m_pNumUniSpr->setPosition(visibleSize.width / 2, fCurY);
	}
	fCurX += numSize.width + 10;

	float fStarPadding = 4;
	fCurY = CThemeScene::Level_Height / 2 - starSize.height / 2 - fStarPadding / 2;
	m_pStarSpr[0]->setPosition(fCurX + numSize.width / 2, fCurY);

	fCurY = CThemeScene::Level_Height / 2 + starSize.height / 2 + fStarPadding / 2;
	m_pStarSpr[2]->setPosition(fCurX + numSize.width / 2, fCurY);
	
	fCurX += starSize.width + starSize.height / 2;
	m_pStarSpr[1]->setPosition(fCurX, CThemeScene::Level_Height / 2);

	m_pLockSpr->setPosition(visibleSize.width / 2, CThemeScene::Level_Height / 2);
}

void LevelSpr::SetStar(int iScore)
{
	for (int i = 0; i < iScore; i++)
	{
		m_pStarSpr[i]->setVisible(true);
	}

	for (int i = iScore; i < 3; i++)
	{
		m_pStarSpr[i]->setVisible(false);
	}
}


bool LevelSpr::GetLockState()
{
	return m_bLockState;
}