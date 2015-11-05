#include "GameScene.h"
#include "DataManager.h"
#include "ResManager.h"
#include "PauseScene.h"
#include "BarrierLayer.h"
#include "PassScene.h"
#include "FailScene.h"
#include "MenuScene.h"

USING_NS_CC;

CGameScene::CGameScene() : m_iClickUserItemIndex(-1)
{
}


CGameScene::~CGameScene()
{

}


//cocos function
Scene* CGameScene::CreateScene()
{
	auto layer = CGameScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
}

//cocos function
bool CGameScene::init()
{
	if (!LayerColor::initWithColor(Color4B(245, 222, 190, 255)))
	{
		return false;
	}

	m_visibleSize = Director::getInstance()->getVisibleSize();

	SetupLevel();

	//CreateBackground();

	//关卡信息
	float fTopHeight = InitLevelInfo();

	//Beat信息
	fTopHeight = InitBeatInfo(fTopHeight);

	//创建底部菜单
	float fBottomHeight = CreateUserItemMenu();

	//六边形地图
	InitHexagon(fTopHeight, fBottomHeight);

	//点击监听
	CreateTouchListener();

	return true;
}

//获取选择的关卡并设置
void CGameScene::SetupLevel()
{
	//获取选择的关卡
	m_iCurTheme = UserDefault::getInstance()->getIntegerForKey("Theme");
	m_iCurLevel = UserDefault::getInstance()->getIntegerForKey("Level");

	//设置当前主题和关卡
	CDataManager::getInstance()->SetCurThemeAndLevel(m_iCurTheme, m_iCurLevel);

	m_iBeatTime = CDataManager::getInstance()->GetMapInfo()->iBeat;
}


void CGameScene::CreateBackground()
{
	//背景
	auto bgSprite = Sprite::create("images/background.png");
	bgSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	addChild(bgSprite);
}


//初始化显示关卡信息
float CGameScene::InitLevelInfo()
{
	float fScreenWidth = m_visibleSize.width;
	float fScreenHeight = m_visibleSize.height;

	auto puzzleSprite = Sprite::create("images/puzzle.png");
	Size puzzleSize = puzzleSprite->getContentSize();
	//数字起始位置
	float fCurX = puzzleSize.width + 15;
	float fCurY = puzzleSize.height / 2;
	float fNumPadding = 2;

	int arrNum[5] = {0};
	arrNum[0] = (m_iCurTheme + 1) / 10;		//主题十位
	arrNum[1] = (m_iCurTheme + 1) % 10;		//主题个位
	arrNum[2] = 11;							//间隔符
	arrNum[3] = (m_iCurLevel + 1) / 10;		//关卡十位
	arrNum[4] = (m_iCurLevel + 1) % 10;		//关卡个位
	//显示主题-关卡信息
	char name[20] = { '\0' };
	for (int i = 0; i < 5; i++)
	{
		sprintf(name, "number_g_%d.png", arrNum[i]);
		auto sprite = Sprite::createWithSpriteFrameName(name);
		float fNumWidth = sprite->getContentSize().width;
		puzzleSprite->addChild(sprite);
		sprite->setPosition(fCurX + fNumWidth / 2, fCurY);

		//下一个数字位置，最后一个则不加fNumPadding
		fCurX += fNumWidth;
		if (i < 4)
		{
			fCurX += fNumPadding;
		}
	}

	//puzzleSprite整体到屏幕左右侧的间距
	float fPuzzlePadding = (fScreenWidth - fCurX) / 2;
	float fPuzzlePosX = fPuzzlePadding + puzzleSize.width / 2;
	//设置puzzleSprite位置
	puzzleSprite->setPosition(fPuzzlePosX, fScreenHeight * 0.85f);
	addChild(puzzleSprite);

	//创建游戏暂停菜单
	auto pauseMenuItem = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_pause.png"),
		Sprite::createWithSpriteFrameName("btn_pause.png"),
		CC_CALLBACK_1(CGameScene::OnButtonClick, this, 0)
		);
	Size pauseSize = pauseMenuItem->getContentSize();
	//puzzleSprite整体与屏幕之间的间距减去暂停菜单宽度的一半
	//即暂停按钮到屏幕右侧之间的间距
	float fPausePadding = (fPuzzlePadding - pauseSize.width) / 2;
	float fPausePosX = fScreenWidth - fPausePadding - pauseSize.width / 2;
	pauseMenuItem->setPosition(fPausePosX, fScreenHeight * 0.85f);

	//创建菜单
	auto menu = Menu::create(pauseMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	//剩余高度
	float fTopHeight = fScreenHeight * 0.85f - pauseSize.width - 5;
	return fTopHeight;
}

//初始化显示Beat次数信息
float CGameScene::InitBeatInfo(float fTopHeight)
{
	//Beat次数显示
	auto beatNode = Node::create();

	//下一个图形位置
	float fCurX = 0;
	float fCurY = 0;

	//1表示六边形，0表示直线
	int arrBeatSprite[7] = { 1, 0, 0, 1, 0, 0, 1 };
	float fStarWidth = 0;
	for (int i = 0; i < 7; i++)
	{
		Sprite* sprite = nullptr;
		if (arrBeatSprite[i] == 1)
		{
			sprite = Sprite::create("images/star.png");
			fStarWidth = sprite->getContentSize().width;
		}
		else
		{
			sprite = Sprite::createWithSpriteFrameName("number_r_11.png");
		}

		//集合
		beatNode->addChild(sprite);
		//添加到序列
		m_vecBeatSpr.pushBack(sprite);
		//设置位置
		float iSpriteWidth = sprite->getContentSize().width;
		sprite->setPosition(fCurX + iSpriteWidth / 2, fCurY);
		//下一个图形X位置
		fCurX += iSpriteWidth;
	}

	//间距
	fCurX += 10;

	//Beat文字
	auto beatTextSprite = Sprite::create("images/beat.png");
	float fBeatTextWidth = beatTextSprite->getContentSize().width;
	beatNode->addChild(beatTextSprite);
	beatTextSprite->setPosition(fCurX + fBeatTextWidth / 2, fCurY);

	//下一个图形位置
	float fNumPadding = 2;
	fCurX += fBeatTextWidth + fNumPadding * 4;

	//Sprite名称数组
	char arrSpriteName[20] = { '\0' };

	//Beat次数十位
	sprintf(arrSpriteName, "number_g_%d.png", abs(m_iBeatTime) / 10);
	m_pBeatDecadeSpr = Sprite::createWithSpriteFrameName(arrSpriteName);
	beatNode->addChild(m_pBeatDecadeSpr);
	float fBeatNumWidth = m_pBeatDecadeSpr->getContentSize().width;
	m_pBeatDecadeSpr->setPosition(fCurX + fBeatNumWidth / 2, fCurY);
	//下一个数字X位置
	fCurX += fBeatNumWidth + fNumPadding;

	//Beat次数个位
	sprintf(arrSpriteName, "number_g_%d.png", abs(m_iBeatTime) % 10);
	m_pBeatUnitSpr = Sprite::createWithSpriteFrameName(arrSpriteName);
	beatNode->addChild(m_pBeatUnitSpr);
	fBeatNumWidth = m_pBeatUnitSpr->getContentSize().width;
	m_pBeatUnitSpr->setPosition(fCurX + fBeatNumWidth / 2, fCurY);
	fCurX += fBeatNumWidth;

	//显示，设置位置
	float fBeatNumHeight = m_pBeatUnitSpr->getContentSize().height;
	float fScreenWidth = m_visibleSize.width;
	beatNode->setPosition((fScreenWidth - fCurX) / 2, fTopHeight - fBeatNumHeight / 2);
	addChild(beatNode);

	return fTopHeight - fStarWidth;
}

//初始化地图
void CGameScene::InitHexagon(float fTopHeight, float fBottomHeight)
{
	//当前高度
	float fCurHeight = fTopHeight;
	//每个六边形高度
	Size hexagonSize = GetHexagonSize();
	//整个屏幕宽度
	float fScreenWidth = m_visibleSize.width;
	//上下左右六边形之间间距
	float fHexagonPadding = 4;

	//获取地图信息
	const MapInfo* pMapInfo = CDataManager::getInstance()->GetMapInfo();

	//左侧空白区域最小宽度
	float fLeftBlankMinWidth = fScreenWidth;
	//右侧空白区域最小宽度
	float fRightBlankMinWidth = fScreenWidth;

	//初始化有效行起始索引为-1
	m_iValidRowStart = -1;
	//初始化有效行数为0
	m_iValidRowNum = 0;

	//六边形集合Node
	auto hexagonGatherNode = Node::create();

	//根据当前地图初始化Hexagon序列
	int iIndex = 0;
	for (int i = 0; i < ROW_MAX; i++)
	{
		//当前行有效标志
		bool bRowValidFlag = false;
		//一行的六边形个数，偶数行比奇数行少1个
		int iColumnCount = COLUMN_MAX;
		if (i % 2 != 0)
		{
			iColumnCount = COLUMN_MAX - 1;
		}

		//起始位置
		float fCurX = (fScreenWidth - hexagonSize.width * iColumnCount - fHexagonPadding * (iColumnCount - 1)) / 2;
		float fCurY = fCurHeight - hexagonSize.height / 2;

		for (int j = 0; j < iColumnCount; j++)
		{
			int iNum = pMapInfo->vecMap[iIndex];
#ifdef _DEBUG_
			log("[%d, %d]:%d", i, j, iNum);
#endif
			if (iNum <= 0)
			{
				m_arrHexagon[i][j].SetValid(false);
			}
			else
			{
				//初始化CHexagon对象
				m_arrHexagon[i][j].Init(iNum);
				//设置有效
				m_arrHexagon[i][j].SetValid(true);
				//获得集合的Node
				auto hexagonNode = m_arrHexagon[i][j].GetGatherNode();
				hexagonNode->setPosition(fCurX + hexagonSize.width / 2, fCurY);
				hexagonGatherNode->addChild(hexagonNode);

				//找左侧最小的空白区域宽度
				if (fLeftBlankMinWidth > fCurX)
				{
					log("Left: %f <=> %f", fLeftBlankMinWidth, fCurX);
					fLeftBlankMinWidth = fCurX;
				}
				//找右侧最小的空白区域宽度
				if (fRightBlankMinWidth > fScreenWidth - fCurX - hexagonSize.width)
				{
					log("Right: %f <=> %f", fRightBlankMinWidth, fScreenWidth - fCurX - hexagonSize.width);
					fRightBlankMinWidth = fScreenWidth - fCurX - hexagonSize.width;
				}

				//行有效
				bRowValidFlag = true;
			}

			fCurX += hexagonSize.width;
			if (j < iColumnCount - 1)
			{
				fCurX += fHexagonPadding;
			}

			iIndex++;
		}
		//有效行处理
		if (bRowValidFlag)
		{
			//如果起始有效行还没获取到，则设置为当前有效行
			if (m_iValidRowStart < 0)
			{
				m_iValidRowStart = i;
			}
			m_iValidRowNum++;

			//重新计算当前高度
			fCurHeight = fCurHeight - hexagonSize.height * 3 / 4 - fHexagonPadding;
		}
	}


	//设置MapItem
	for (int i = 0; i < pMapInfo->vecMapItem.size(); i++)
	{
		int iRowIdx = pMapInfo->vecMapItem[i].iX;
		int iColIdx = pMapInfo->vecMapItem[i].iY;
		int iArrowType = pMapInfo->vecMapItem[i].iType;
		if (!m_arrHexagon[iRowIdx][iColIdx].GetValid())
		{
			log("Hexagon in pos(%d, %d) is invalid. iType=%d", iRowIdx, iColIdx, iArrowType);
			continue;
		}

		log("Set hexagon in pos(%d, %d) arrow type=%d", iRowIdx, iColIdx, iArrowType);
		m_arrHexagon[iRowIdx][iColIdx].SetArrow(iArrowType, true);
	}


	//两侧的最小空白区域宽度要相等
	log("%f, %f", fLeftBlankMinWidth, fRightBlankMinWidth);
	m_fGatherOffset.x = (fRightBlankMinWidth - fLeftBlankMinWidth) / 2;
	m_fGatherOffset.y = -(fTopHeight - fBottomHeight - (fTopHeight - fCurHeight) - fHexagonPadding) / 2;
	log("fTopHeight=%f, fBottomHeight=%f, fMapHeight=%f  m_fGatherOffsetY=%f", fTopHeight, fBottomHeight, fCurHeight, m_fGatherOffset.y);
	//是否需要执行动作
	bool bActionFlag = UserDefault::getInstance()->getBoolForKey("HexagonAction", true);
	if (bActionFlag)
	{
		hexagonGatherNode->setPosition(m_fGatherOffset + Vec2(fScreenWidth, 0));
		//从右向左滑入
		hexagonGatherNode->runAction(MoveTo::create(0.2f, m_fGatherOffset));
		//恢复标志
		UserDefault::getInstance()->setBoolForKey("HexagonAction", true);
	}
	else
	{
		hexagonGatherNode->setPosition(m_fGatherOffset);
	}

	addChild(hexagonGatherNode);
}

//初始化底部UserItem菜单
float CGameScene::CreateUserItemMenu()
{
	float fScreenWidth = m_visibleSize.width;

	//获取地图信息
	const MapInfo* pMapInfo = CDataManager::getInstance()->GetMapInfo();

	//菜单项集合
	Vector<MenuItem *> vecMenuItem;

	//间距
	float fPadding = 0;
	//当前位置
	float fCurX = 0;
	float fCurY = 0;

	//UserItem最多个数和实际个数
	int iTotalNum = 5;
	int iActualNum = pMapInfo->vecUserItem.size();

	for (int i = 0; i < iTotalNum; i++)
	{
		auto bgSprite = Sprite::create("images/itemBK.png");
		//默认无效
		m_oUserItem[i].SetValid(false);

		//实际UserItem个数可能不足5个
		if (i < iActualNum)
		{
			//添加箭头
			m_oUserItem[i].SetArrowType(pMapInfo->vecUserItem[i]);
			char name[20] = { '\0' };
			sprintf(name, "item_%d_w.png", m_oUserItem[i].GetArrowType());
			m_oUserItem[i].m_pArrowSpr = Sprite::createWithSpriteFrameName(name);
			bgSprite->addChild(m_oUserItem[i].m_pArrowSpr);

			//设置箭头位置
			Size bgSize = bgSprite->getContentSize();
			m_oUserItem[i].m_pArrowSpr->setPosition(bgSize.width / 2, bgSize.height / 2);
			
			//设置点击时显示的框框
			m_oUserItem[i].m_pClickSpr = Sprite::create("images/itemchoose.png");
			bgSprite->addChild(m_oUserItem[i].m_pClickSpr);
			m_oUserItem[i].m_pClickSpr->setPosition(bgSize.width / 2, bgSize.height / 2);

			//默认未点击
			m_oUserItem[i].OnUnclicked();

			//设置有效
			m_oUserItem[i].SetValid(true);
		}
		
		//sprite宽高
		float fSpriteWidth = bgSprite->getContentSize().width;
		float fSpriteHeight = bgSprite->getContentSize().height;

		if (fabs(fPadding) < 1e-6)
		{
			fPadding = (fScreenWidth - fSpriteWidth * iTotalNum) / (iTotalNum + 1);
			fCurX += fPadding;
			fCurY = fSpriteHeight * 1.2f;
		}

		//创建菜单
		auto menuItem = MenuItemSprite::create(
			bgSprite,
			bgSprite,
			CC_CALLBACK_1(CGameScene::OnUserItemClick, this, i)
		);
		menuItem->setPosition(fCurX + fSpriteWidth / 2, fCurY);
		vecMenuItem.pushBack(menuItem);

		//下一个UserItem位置
		fCurX += fSpriteWidth + fPadding;
	}

	auto menu = Menu::createWithArray(vecMenuItem);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	return fCurY * 1.8f / 1.2f;
}


//创建触摸监听器
void CGameScene::CreateTouchListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		//点击需要有间隔，避免Beat次数或六边形动作还没执行完
		//就开始执行下次动作，导致出现了缩放。
		if (!m_bCanClicked)
		{
			return false;
		}

		//设置不可点击
		m_bCanClicked = false;

		//获取的当前触摸的目标
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		//获取触摸点位置
		Vec2 touchPos = target->convertToNodeSpace(touch->getLocation());
		return CheckHexagonClick(touchPos);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}


//检查是哪个六边形被点击
bool CGameScene::CheckHexagonClick(cocos2d::Vec2 touchPos)
{
	for (int i = 0; i < ROW_MAX; i++)
	{
		int iColCount = COLUMN_MAX + ((i % 2 == 0) ? 0 : -1);
		for (int j = 0; j < iColCount; j++)
		{
			if (m_arrHexagon[i][j].GetValid())
			{
				Size nodeSize = GetHexagonSize();
				Vec2 hexagonPos = m_arrHexagon[i][j].GetGatherNode()->getPosition();
				Rect hexagonRect = Rect(hexagonPos.x + m_fGatherOffset.x - nodeSize.width / 2, hexagonPos.y
					- nodeSize.height / 2 + m_fGatherOffset.y, nodeSize.width, nodeSize.height);
				if (hexagonRect.containsPoint(touchPos))
				{
					OnHexagonClick(i, j);
					return true;
				}
			}
		}
	}

	return false;
}


//Hexagon索引类
class HexagonIndex
{
public:
	int m_iRowIndex;
	int m_iColIndex;

	HexagonIndex(int iRowIndex, int iColIndex) : m_iRowIndex(iRowIndex), m_iColIndex(iColIndex)
	{

	}

	bool IsValid(int iRowCount, int iColCount)
	{
		if (m_iRowIndex >= 0 && m_iRowIndex < iRowCount && m_iColIndex >= 0 && m_iColIndex < iColCount)
		{
			return true;
		}

		return false;
	}
};


//六边形点击处理
void CGameScene::OnHexagonClick(int iRowIndex, int iColIndex)
{
#ifdef _DEBUG_
	log("OnHexagonClick:%d %d", iRowIndex, iColIndex);
#endif

	//获取箭头和数字
	int iArrow = m_arrHexagon[iRowIndex][iColIndex].GetArrow();
	int iNum = m_arrHexagon[iRowIndex][iColIndex].GetNum();
	
	//点击了UserItem
	if (m_iClickUserItemIndex > -1)
	{
		//箭头不存在且无数字
		if (iArrow == HexagonArrow_None && iNum == 0)
		{
			int iArrowType = m_oUserItem[m_iClickUserItemIndex].GetArrowType();
			//设置箭头
			m_arrHexagon[iRowIndex][iColIndex].SetArrow(iArrowType, true);
			//UserItem已使用
			m_oUserItem[m_iClickUserItemIndex].OnUsed();
			//重置索引
			m_iClickUserItemIndex = -1;
		}
		return;
	}

	//根据点击的六边形是否有箭头进行分别处理
	if (iArrow > HexagonArrow_None && iArrow <= HexagonArrow_Hexagon)
	{
		//播放声效
		CResManager::getInstance()->PlayEffect("sounds/sound_item.mp3");

		ChangeWithArrow(iRowIndex, iColIndex, iArrow);
	}
	else
	{
		//播放声效
		CResManager::getInstance()->PlayEffect("sounds/sound_change.mp3");

		ChangeWithoutArrow(iRowIndex, iColIndex);
	}
	

	//更新Beat次数
	ChangeBeatTime();

	//0.32秒后检查是否通关
	scheduleOnce(schedule_selector(CGameScene::CheckGameOver), 0.30f);
}



void CGameScene::OnButtonClick(Ref* pSender, int iIndex)
{
#ifdef _DEBUG_
	log("OnButtonClick %d", iIndex);
#endif

	//点击暂停按钮
	if (0 == iIndex)
	{
		//不需要执行动作
		UserDefault::getInstance()->setBoolForKey("HexagonAction", false);

		auto layer = CBarrierLayer::create();
		layer->addChild(CPauseScene::create());
		addChild(layer);
		Director::getInstance()->pause();
	}
}


//UserItem点击响应
void CGameScene::OnUserItemClick(Ref* pSender, int iIndex)
{
	if (!m_oUserItem[iIndex].IsValid())
	{
		return;
	}

	m_oUserItem[iIndex].OnClicked();
	bool bVisible = m_oUserItem[iIndex].IsClicked();
	//如果是显示的，隐藏其他UserItem的点击
	if (bVisible)
	{
		for (int i = 0; i < 5; i++)
		{
			if (!m_oUserItem[i].IsValid() || (iIndex == i))
			{
				continue;
			}

			m_oUserItem[i].OnUnclicked();
		}
	}

#ifdef _DEBUG_
	log("m_iClickUserItemIndex: %d->%d", m_iClickUserItemIndex, bVisible ? iIndex : -1);
#endif

	m_iClickUserItemIndex = bVisible ? iIndex : -1;
}


//Beat次数改变
void CGameScene::ChangeBeatTime()
{
#ifdef _DEBUG_
	log("BeatAction: m_iBeatTime, %d->%d", m_iBeatTime, m_iBeatTime - 1);
#endif

	m_iBeatTime -= 1;

	//当剩余次数达到对应值时隐藏对应位置Sprite
	int arrBeatTime[] = {-10, -8, -6, -5, -4, -2, -1};
	for (int i = 0; i < 7; i++)
	{
		if (m_iBeatTime == arrBeatTime[i])
		{
			m_vecBeatSpr.at(i)->setVisible(false);
		}
	}


	bool bExtraFlag = (m_iBeatTime < 0);
	//设置十位缩放动作
	auto decadeScaleBy = CCScaleBy::create(0.13f, 2);
	auto decadeScaleBack = decadeScaleBy->reverse();
	m_pBeatDecadeSpr->runAction(
		Sequence::create(
			decadeScaleBy,
			CallFuncN::create(CC_CALLBACK_1(CGameScene::OnBeatActionCallback, this, bExtraFlag, abs(m_iBeatTime / 10))),
			decadeScaleBack,
			NULL
		)
	);
	//设置个位缩放动作
	auto unitScaleBy = CCScaleBy::create(0.13f, 2);
	auto unitScaleBack = unitScaleBy->reverse();
	m_pBeatUnitSpr->runAction(
		Sequence::create(
			unitScaleBy,
			CallFuncN::create(CC_CALLBACK_1(CGameScene::OnBeatActionCallback, this, bExtraFlag, abs(m_iBeatTime % 10))),
			unitScaleBack,
			NULL
		)
	);
}


//Beat次数执行动作调用的回调函数
//bExtraFlag表示是否是额外次数，额外次数用红色表示
void CGameScene::OnBeatActionCallback(Node* pSender, bool bExtraFlag, int iNum)
{
	char name[20] = {'\0'};
	if (bExtraFlag)
	{
		sprintf(name, "number_r_%d.png", iNum);
	}
	else
	{
		sprintf(name, "number_g_%d.png", iNum);
	}
	Sprite* sprite = (Sprite *)pSender;
	sprite->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(name));
}


Size CGameScene::GetHexagonSize()
{
	return Size(60, 63);
}


void CGameScene::CheckGameOver(float dt)
{
	//设置可点击
	m_bCanClicked = true;

	bool bWinFlag = true;
	for (int i = m_iValidRowStart; i < m_iValidRowStart + m_iValidRowNum; i++)
	{
		int iColCount = ((i % 2 == 0) ? COLUMN_MAX : COLUMN_MAX - 1);
		for (int j = 0; j < iColCount; j++)
		{
			if (m_arrHexagon[i][j].GetValid() && !m_arrHexagon[i][j].GetReady())
			{
				bWinFlag = false;
				goto RESULT;
			}
		}
	}
	goto RESULT;


RESULT:
	bool bPassFlag = false;
	if (bWinFlag && m_iBeatTime >= BEAT_TIME_LIMIT)
	{
		CResManager::getInstance()->PlayEffect("sounds/sound_complete.mp3");
		//定时3秒触发成功或失败界面
		this->runAction(
			Sequence::create(
			DelayTime::create(0.8f),
			CallFuncN::create(CC_CALLBACK_1(CGameScene::OnTimeCallback, this, true)),
			NULL
			)
		);
	}
	else if (m_iBeatTime < BEAT_TIME_LIMIT || 
		(m_iBeatTime == BEAT_TIME_LIMIT && !bWinFlag))
	{
		CResManager::getInstance()->PlayEffect("sounds/sound_dead.mp3");
		OnTimeCallback(nullptr, false);
	}
}


void CGameScene::LevelPass()
{
#ifdef _DEBUG_
	log("LevelPass.");
#endif

	int iScore = 0;
	if (m_iBeatTime >= 0)
	{
		iScore = 3;
	}
	else if (m_iBeatTime < 0 && m_iBeatTime > -5)
	{
		iScore = 2;
	}
	else if (m_iBeatTime <= -5)
	{
		iScore = 1;
	}

	//记录本次分数
	UserDefault::getInstance()->setIntegerForKey("Score", iScore);
	//通关
	CDataManager::getInstance()->Pass(iScore);

	//切换到通关界面
	auto scene = CPassScene::CreateScene();
	Director::getInstance()->replaceScene(scene);

}

void CGameScene::LevelFail()
{
#ifdef _DEBUG_
	log("LevelFail.");
#endif

	//切换到失败界面
	auto scene = CFailScene::CreateScene();
	Director::getInstance()->replaceScene(scene);

}



//箭头转换表
const int arrArrowChangeList[5][6] =
{
	{ 0, 0, 0, 0, 0, 0 },
	{ 3, 2, 1, 1, 2, 3 },
	{ 2, 1, 3, 3, 1, 2 },
	{ 1, 3, 2, 2, 3, 1 },
	{ 4, 4, 4, 4, 4, 4 }
};

//获取改变之后的箭头类型，iPos表示是第几个位置，iCurArrowType表示当前箭头类型
int CGameScene::GetChangedArrow(int iPos, int iCurArrowType)
{
	return arrArrowChangeList[iCurArrowType][iPos];
}


//点击的位置包含箭头
void CGameScene::ChangeWithArrow(int iRowIndex, int iColIndex, int iArrow)
{
	if (iArrow == HexagonArrow_Hexagon)
	{
		//改变自己
		m_arrHexagon[iRowIndex][iColIndex].Change(true);
		return;
	}

	if (iArrow == HexagonArrow_Transverse)
	{
		//一行最多个数
		int iColCount = COLUMN_MAX + ((iRowIndex % 2 == 0) ? 0 : -1);
		for (int j = 0; j < iColCount; j++)
		{
			if (m_arrHexagon[iRowIndex][j].GetValid())
			{
				m_arrHexagon[iRowIndex][j].Change(j == iColIndex);
			}
		}
		return;
	}

	//改变自己
	m_arrHexagon[iRowIndex][iColIndex].Change(true);

	if (iArrow == HexagonArrow_LeftTop)
	{
		int iTemp = ((iRowIndex % 2 == 0) ? 0 : 1);
		for (int i = iRowIndex + 1; i < ROW_MAX; i++)
		{
			//一行最多个数
			int iColCount = COLUMN_MAX + ((i % 2 == 0) ? 0 : -1);
			int iColIdx = iColIndex + ((i - iRowIndex + iTemp) / 2);

			if (iColIdx >= iColCount)
			{
				break;
			}

			if (m_arrHexagon[i][iColIdx].GetValid())
			{
				m_arrHexagon[i][iColIdx].Change(false);
			}
		}

		iTemp = ((iRowIndex % 2 == 0) ? 1 : 0);
		for (int i = iRowIndex - 1; i >= 0; i--)
		{
			//一行最多个数
			int iColCount = COLUMN_MAX + ((i % 2 == 0) ? 0 : -1);
			int iColIdx = iColIndex - ((iRowIndex - i + iTemp) / 2);
			if (iColIdx < 0)
			{
				break;
			}

			if (m_arrHexagon[i][iColIdx].GetValid())
			{
				m_arrHexagon[i][iColIdx].Change(false);
			}
		}

		return;
	}

	if (iArrow == HexagonArrow_RightTop)
	{
		int iTemp = ((iRowIndex % 2 == 0) ? 1 : 0);
		for (int i = iRowIndex + 1; i < ROW_MAX; i++)
		{
			//一行最多个数
			int iColCount = COLUMN_MAX + ((i % 2 == 0) ? 0 : -1);
			int iColIdx = iColIndex - ((i - iRowIndex + iTemp) / 2);
			if (iColIdx < 0)
			{
				break;
			}

			if (m_arrHexagon[i][iColIdx].GetValid())
			{
				m_arrHexagon[i][iColIdx].Change(false);
			}
		}

		iTemp = ((iRowIndex % 2 == 0) ? 0 : 1);
		for (int i = iRowIndex - 1; i >= 0; i--)
		{
			//一行最多个数
			int iColCount = COLUMN_MAX + ((i % 2 == 0) ? 0 : -1);
			int iColIdx = iColIndex + ((iRowIndex - i + iTemp) / 2);
			if (iColIdx >= iColCount)
			{
				break;
			}

			if (m_arrHexagon[i][iColIdx].GetValid())
			{
				m_arrHexagon[i][iColIdx].Change(false);
			}
		}
	}
}

//点击的位置没有箭头
void CGameScene::ChangeWithoutArrow(int iRowIndex, int iColIndex)
{
	//改变自己
	m_arrHexagon[iRowIndex][iColIndex].Change(true);

	HexagonIndex arrHexagonIdx[6] = {
		HexagonIndex(iRowIndex - 1, (iRowIndex % 2 != 0) ? iColIndex : iColIndex - 1),
		HexagonIndex(iRowIndex - 1, (iRowIndex % 2 != 0) ? iColIndex + 1 : iColIndex),
		HexagonIndex(iRowIndex, iColIndex - 1),
		HexagonIndex(iRowIndex, iColIndex + 1),
		HexagonIndex(iRowIndex + 1, (iRowIndex % 2 != 0) ? iColIndex : iColIndex - 1),
		HexagonIndex(iRowIndex + 1, (iRowIndex % 2 != 0) ? iColIndex + 1 : iColIndex),
	};

	for (int i = 0; i < 6; i++)
	{
		int iRowIdx = arrHexagonIdx[i].m_iRowIndex;
		int iColIdx = arrHexagonIdx[i].m_iColIndex;
		//一行最多个数
		int iColCount = COLUMN_MAX + ((iRowIdx % 2 == 0) ? 0 : -1);
		if (arrHexagonIdx[i].IsValid(ROW_MAX, iColCount) && m_arrHexagon[iRowIdx][iColIdx].GetValid())
		{
			int iArrowType = m_arrHexagon[iRowIdx][iColIdx].GetArrow();
			int iChangedArrowType = GetChangedArrow(i, iArrowType);
			log("[%d][%d]: iArrowType=%d->%d", iRowIdx, iColIdx, iArrowType, iChangedArrowType);
			if (iChangedArrowType == iArrowType)
			{
				//没有改变时置为None
				iChangedArrowType = HexagonArrow_None;
			}
			m_arrHexagon[iRowIdx][iColIdx].Change(false, iChangedArrowType);
		}
	}
}


//定时切换成功或失败界面
void CGameScene::OnTimeCallback(Node* pSender, bool bPassFlag)
{
	if (bPassFlag)
	{
		LevelPass();
	}
	else
	{
		LevelFail();
	}
}
