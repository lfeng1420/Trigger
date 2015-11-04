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

	//�ؿ���Ϣ
	float fTopHeight = InitLevelInfo();

	//Beat��Ϣ
	fTopHeight = InitBeatInfo(fTopHeight);

	//�����ײ��˵�
	float fBottomHeight = CreateUserItemMenu();

	//�����ε�ͼ
	InitHexagon(fTopHeight, fBottomHeight);

	//�������
	CreateTouchListener();

	return true;
}

//��ȡѡ��Ĺؿ�������
void CGameScene::SetupLevel()
{
	//��ȡѡ��Ĺؿ�
	m_iCurTheme = UserDefault::getInstance()->getIntegerForKey("Theme");
	m_iCurLevel = UserDefault::getInstance()->getIntegerForKey("Level");

	//���õ�ǰ����͹ؿ�
	CDataManager::getInstance()->SetCurThemeAndLevel(m_iCurTheme, m_iCurLevel);

	m_iBeatTime = CDataManager::getInstance()->GetMapInfo()->iBeat;
}


void CGameScene::CreateBackground()
{
	//����
	auto bgSprite = Sprite::create("images/background.png");
	bgSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	addChild(bgSprite);
}


//��ʼ����ʾ�ؿ���Ϣ
float CGameScene::InitLevelInfo()
{
	float fScreenWidth = m_visibleSize.width;
	float fScreenHeight = m_visibleSize.height;

	auto puzzleSprite = Sprite::create("images/puzzle.png");
	Size puzzleSize = puzzleSprite->getContentSize();
	//������ʼλ��
	float fCurX = puzzleSize.width + 15;
	float fCurY = puzzleSize.height / 2;
	float fNumPadding = 2;

	int arrNum[5] = {0};
	arrNum[0] = (m_iCurTheme + 1) / 10;		//����ʮλ
	arrNum[1] = (m_iCurTheme + 1) % 10;		//�����λ
	arrNum[2] = 11;							//�����
	arrNum[3] = (m_iCurLevel + 1) / 10;		//�ؿ�ʮλ
	arrNum[4] = (m_iCurLevel + 1) % 10;		//�ؿ���λ
	//��ʾ����-�ؿ���Ϣ
	char name[20] = { '\0' };
	for (int i = 0; i < 5; i++)
	{
		sprintf(name, "number_g_%d.png", arrNum[i]);
		auto sprite = Sprite::createWithSpriteFrameName(name);
		float fNumWidth = sprite->getContentSize().width;
		puzzleSprite->addChild(sprite);
		sprite->setPosition(fCurX + fNumWidth / 2, fCurY);

		//��һ������λ�ã����һ���򲻼�fNumPadding
		fCurX += fNumWidth;
		if (i < 4)
		{
			fCurX += fNumPadding;
		}
	}

	//puzzleSprite���嵽��Ļ���Ҳ�ļ��
	float fPuzzlePadding = (fScreenWidth - fCurX) / 2;
	float fPuzzlePosX = fPuzzlePadding + puzzleSize.width / 2;
	//����puzzleSpriteλ��
	puzzleSprite->setPosition(fPuzzlePosX, fScreenHeight * 0.85f);
	addChild(puzzleSprite);

	//������Ϸ��ͣ�˵�
	auto pauseMenuItem = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("btn_pause.png"),
		Sprite::createWithSpriteFrameName("btn_pause.png"),
		CC_CALLBACK_1(CGameScene::OnButtonClick, this, 0)
		);
	Size pauseSize = pauseMenuItem->getContentSize();
	//puzzleSprite��������Ļ֮��ļ���ȥ��ͣ�˵���ȵ�һ��
	//����ͣ��ť����Ļ�Ҳ�֮��ļ��
	float fPausePadding = (fPuzzlePadding - pauseSize.width) / 2;
	float fPausePosX = fScreenWidth - fPausePadding - pauseSize.width / 2;
	pauseMenuItem->setPosition(fPausePosX, fScreenHeight * 0.85f);

	//�����˵�
	auto menu = Menu::create(pauseMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	//ʣ��߶�
	float fTopHeight = fScreenHeight * 0.85f - pauseSize.width - 5;
	return fTopHeight;
}

//��ʼ����ʾBeat������Ϣ
float CGameScene::InitBeatInfo(float fTopHeight)
{
	//Beat������ʾ
	auto beatNode = Node::create();

	//��һ��ͼ��λ��
	float fCurX = 0;
	float fCurY = 0;

	//1��ʾ�����Σ�0��ʾֱ��
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

		//����
		beatNode->addChild(sprite);
		//��ӵ�����
		m_vecBeatSpr.pushBack(sprite);
		//����λ��
		float iSpriteWidth = sprite->getContentSize().width;
		sprite->setPosition(fCurX + iSpriteWidth / 2, fCurY);
		//��һ��ͼ��Xλ��
		fCurX += iSpriteWidth;
	}

	//���
	fCurX += 10;

	//Beat����
	auto beatTextSprite = Sprite::create("images/beat.png");
	float fBeatTextWidth = beatTextSprite->getContentSize().width;
	beatNode->addChild(beatTextSprite);
	beatTextSprite->setPosition(fCurX + fBeatTextWidth / 2, fCurY);

	//��һ��ͼ��λ��
	float fNumPadding = 2;
	fCurX += fBeatTextWidth + fNumPadding * 4;

	//Sprite��������
	char arrSpriteName[20] = { '\0' };

	//Beat����ʮλ
	sprintf(arrSpriteName, "number_g_%d.png", abs(m_iBeatTime) / 10);
	m_pBeatDecadeSpr = Sprite::createWithSpriteFrameName(arrSpriteName);
	beatNode->addChild(m_pBeatDecadeSpr);
	float fBeatNumWidth = m_pBeatDecadeSpr->getContentSize().width;
	m_pBeatDecadeSpr->setPosition(fCurX + fBeatNumWidth / 2, fCurY);
	//��һ������Xλ��
	fCurX += fBeatNumWidth + fNumPadding;

	//Beat������λ
	sprintf(arrSpriteName, "number_g_%d.png", abs(m_iBeatTime) % 10);
	m_pBeatUnitSpr = Sprite::createWithSpriteFrameName(arrSpriteName);
	beatNode->addChild(m_pBeatUnitSpr);
	fBeatNumWidth = m_pBeatUnitSpr->getContentSize().width;
	m_pBeatUnitSpr->setPosition(fCurX + fBeatNumWidth / 2, fCurY);
	fCurX += fBeatNumWidth;

	//��ʾ������λ��
	float fBeatNumHeight = m_pBeatUnitSpr->getContentSize().height;
	float fScreenWidth = m_visibleSize.width;
	beatNode->setPosition((fScreenWidth - fCurX) / 2, fTopHeight - fBeatNumHeight / 2);
	addChild(beatNode);

	return fTopHeight - fStarWidth;
}

//��ʼ����ͼ
void CGameScene::InitHexagon(float fTopHeight, float fBottomHeight)
{
	//��ǰ�߶�
	float fCurHeight = fTopHeight;
	//ÿ�������θ߶�
	Size hexagonSize = GetHexagonSize();
	//������Ļ���
	float fScreenWidth = m_visibleSize.width;
	//��������������֮����
	float fHexagonPadding = 4;

	//��ȡ��ͼ��Ϣ
	const MapInfo* pMapInfo = CDataManager::getInstance()->GetMapInfo();

	//���հ�������С���
	float fLeftBlankMinWidth = fScreenWidth;
	//�Ҳ�հ�������С���
	float fRightBlankMinWidth = fScreenWidth;

	//��ʼ����Ч����ʼ����Ϊ-1
	m_iValidRowStart = -1;
	//��ʼ����Ч����Ϊ0
	m_iValidRowNum = 0;

	//�����μ���Node
	auto hexagonGatherNode = Node::create();

	//���ݵ�ǰ��ͼ��ʼ��Hexagon����
	int iIndex = 0;
	for (int i = 0; i < ROW_MAX; i++)
	{
		//��ǰ����Ч��־
		bool bRowValidFlag = false;
		//һ�е������θ�����ż���б���������1��
		int iColumnCount = COLUMN_MAX;
		if (i % 2 != 0)
		{
			iColumnCount = COLUMN_MAX - 1;
		}

		//��ʼλ��
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
				//��ʼ��CHexagon����
				m_arrHexagon[i][j].Init(iNum);
				//������Ч
				m_arrHexagon[i][j].SetValid(true);
				//��ü��ϵ�Node
				auto hexagonNode = m_arrHexagon[i][j].GetGatherNode();
				hexagonNode->setPosition(fCurX + hexagonSize.width / 2, fCurY);
				hexagonGatherNode->addChild(hexagonNode);

				//�������С�Ŀհ�������
				if (fLeftBlankMinWidth > fCurX)
				{
					log("Left: %f <=> %f", fLeftBlankMinWidth, fCurX);
					fLeftBlankMinWidth = fCurX;
				}
				//���Ҳ���С�Ŀհ�������
				if (fRightBlankMinWidth > fScreenWidth - fCurX - hexagonSize.width)
				{
					log("Right: %f <=> %f", fRightBlankMinWidth, fScreenWidth - fCurX - hexagonSize.width);
					fRightBlankMinWidth = fScreenWidth - fCurX - hexagonSize.width;
				}

				//����Ч
				bRowValidFlag = true;
			}

			fCurX += hexagonSize.width;
			if (j < iColumnCount - 1)
			{
				fCurX += fHexagonPadding;
			}

			iIndex++;
		}
		//��Ч�д���
		if (bRowValidFlag)
		{
			//�����ʼ��Ч�л�û��ȡ����������Ϊ��ǰ��Ч��
			if (m_iValidRowStart < 0)
			{
				m_iValidRowStart = i;
			}
			m_iValidRowNum++;

			//���¼��㵱ǰ�߶�
			fCurHeight = fCurHeight - hexagonSize.height * 3 / 4 - fHexagonPadding;
		}
	}


	//����MapItem
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


	//�������С�հ�������Ҫ���
	log("%f, %f", fLeftBlankMinWidth, fRightBlankMinWidth);
	m_fGatherOffset.x = (fRightBlankMinWidth - fLeftBlankMinWidth) / 2;
	m_fGatherOffset.y = -(fTopHeight - fBottomHeight - (fTopHeight - fCurHeight) - fHexagonPadding) / 2;
	log("fTopHeight=%f, fBottomHeight=%f, fMapHeight=%f  m_fGatherOffsetY=%f", fTopHeight, fBottomHeight, fCurHeight, m_fGatherOffset.y);
	//�Ƿ���Ҫִ�ж���
	bool bActionFlag = UserDefault::getInstance()->getBoolForKey("HexagonAction", true);
	if (bActionFlag)
	{
		hexagonGatherNode->setPosition(m_fGatherOffset + Vec2(fScreenWidth, 0));
		//����������
		hexagonGatherNode->runAction(MoveTo::create(0.2f, m_fGatherOffset));
		//�ָ���־
		UserDefault::getInstance()->setBoolForKey("HexagonAction", true);
	}
	else
	{
		hexagonGatherNode->setPosition(m_fGatherOffset);
	}

	addChild(hexagonGatherNode);
}

//��ʼ���ײ�UserItem�˵�
float CGameScene::CreateUserItemMenu()
{
	float fScreenWidth = m_visibleSize.width;

	//��ȡ��ͼ��Ϣ
	const MapInfo* pMapInfo = CDataManager::getInstance()->GetMapInfo();

	//�˵����
	Vector<MenuItem *> vecMenuItem;

	//���
	float fPadding = 0;
	//��ǰλ��
	float fCurX = 0;
	float fCurY = 0;

	//UserItem��������ʵ�ʸ���
	int iTotalNum = 5;
	int iActualNum = pMapInfo->vecUserItem.size();

	for (int i = 0; i < iTotalNum; i++)
	{
		auto bgSprite = Sprite::create("images/itemBK.png");
		//Ĭ����Ч
		m_oUserItem[i].SetValid(false);

		//ʵ��UserItem�������ܲ���5��
		if (i < iActualNum)
		{
			//��Ӽ�ͷ
			m_oUserItem[i].SetArrowType(pMapInfo->vecUserItem[i]);
			char name[20] = { '\0' };
			sprintf(name, "item_%d_w.png", m_oUserItem[i].GetArrowType());
			m_oUserItem[i].m_pArrowSpr = Sprite::createWithSpriteFrameName(name);
			bgSprite->addChild(m_oUserItem[i].m_pArrowSpr);

			//���ü�ͷλ��
			Size bgSize = bgSprite->getContentSize();
			m_oUserItem[i].m_pArrowSpr->setPosition(bgSize.width / 2, bgSize.height / 2);
			
			//���õ��ʱ��ʾ�Ŀ��
			m_oUserItem[i].m_pClickSpr = Sprite::create("images/itemchoose.png");
			bgSprite->addChild(m_oUserItem[i].m_pClickSpr);
			m_oUserItem[i].m_pClickSpr->setPosition(bgSize.width / 2, bgSize.height / 2);

			//Ĭ��δ���
			m_oUserItem[i].OnUnclicked();

			//������Ч
			m_oUserItem[i].SetValid(true);
		}
		
		//sprite���
		float fSpriteWidth = bgSprite->getContentSize().width;
		float fSpriteHeight = bgSprite->getContentSize().height;

		if (fabs(fPadding) < 1e-6)
		{
			fPadding = (fScreenWidth - fSpriteWidth * iTotalNum) / (iTotalNum + 1);
			fCurX += fPadding;
			fCurY = fSpriteHeight * 1.2f;
		}

		//�����˵�
		auto menuItem = MenuItemSprite::create(
			bgSprite,
			bgSprite,
			CC_CALLBACK_1(CGameScene::OnUserItemClick, this, i)
		);
		menuItem->setPosition(fCurX + fSpriteWidth / 2, fCurY);
		vecMenuItem.pushBack(menuItem);

		//��һ��UserItemλ��
		fCurX += fSpriteWidth + fPadding;
	}

	auto menu = Menu::createWithArray(vecMenuItem);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	return fCurY * 1.8f / 1.2f;
}


//��������������
void CGameScene::CreateTouchListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		//�����Ҫ�м��������Beat�����������ζ�����ûִ����
		//�Ϳ�ʼִ���´ζ��������³��������š�
		if (!m_bCanClicked)
		{
			return false;
		}

		//���ò��ɵ��
		m_bCanClicked = false;

		//��ȡ�ĵ�ǰ������Ŀ��
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		//��ȡ������λ��
		Vec2 touchPos = target->convertToNodeSpace(touch->getLocation());
		return CheckHexagonClick(touchPos);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}


//������ĸ������α����
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


//Hexagon������
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


//�����ε������
void CGameScene::OnHexagonClick(int iRowIndex, int iColIndex)
{
#ifdef _DEBUG_
	log("OnHexagonClick:%d %d", iRowIndex, iColIndex);
#endif

	//��ȡ��ͷ������
	int iArrow = m_arrHexagon[iRowIndex][iColIndex].GetArrow();
	int iNum = m_arrHexagon[iRowIndex][iColIndex].GetNum();
	
	//�����UserItem
	if (m_iClickUserItemIndex > -1)
	{
		//��ͷ��������������
		if (iArrow == HexagonArrow_None && iNum == 0)
		{
			int iArrowType = m_oUserItem[m_iClickUserItemIndex].GetArrowType();
			//���ü�ͷ
			m_arrHexagon[iRowIndex][iColIndex].SetArrow(iArrowType, true);
			//UserItem��ʹ��
			m_oUserItem[m_iClickUserItemIndex].OnUsed();
			//��������
			m_iClickUserItemIndex = -1;
		}
		return;
	}

	//���ݵ�����������Ƿ��м�ͷ���зֱ���
	if (iArrow > HexagonArrow_None && iArrow <= HexagonArrow_Hexagon)
	{
		//������Ч
		CResManager::getInstance()->PlayEffect("sounds/sound_item.mp3");

		ChangeWithArrow(iRowIndex, iColIndex, iArrow);
	}
	else
	{
		//������Ч
		CResManager::getInstance()->PlayEffect("sounds/sound_change.mp3");

		ChangeWithoutArrow(iRowIndex, iColIndex);
	}
	

	//����Beat����
	ChangeBeatTime();

	//0.32������Ƿ�ͨ��
	scheduleOnce(schedule_selector(CGameScene::CheckGameOver), 0.30f);
}



void CGameScene::OnButtonClick(Ref* pSender, int iIndex)
{
#ifdef _DEBUG_
	log("OnButtonClick %d", iIndex);
#endif

	//�����ͣ��ť
	if (0 == iIndex)
	{
		//����Ҫִ�ж���
		UserDefault::getInstance()->setBoolForKey("HexagonAction", false);

		auto layer = CBarrierLayer::create();
		layer->addChild(CPauseScene::create());
		addChild(layer);
		Director::getInstance()->pause();
	}
}


//UserItem�����Ӧ
void CGameScene::OnUserItemClick(Ref* pSender, int iIndex)
{
	if (!m_oUserItem[iIndex].IsValid())
	{
		return;
	}

	m_oUserItem[iIndex].OnClicked();
	bool bVisible = m_oUserItem[iIndex].IsClicked();
	//�������ʾ�ģ���������UserItem�ĵ��
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


//Beat�����ı�
void CGameScene::ChangeBeatTime()
{
#ifdef _DEBUG_
	log("BeatAction: m_iBeatTime, %d->%d", m_iBeatTime, m_iBeatTime - 1);
#endif

	m_iBeatTime -= 1;

	//��ʣ������ﵽ��Ӧֵʱ���ض�Ӧλ��Sprite
	int arrBeatTime[] = {-10, -8, -6, -5, -4, -2, -1};
	for (int i = 0; i < 7; i++)
	{
		if (m_iBeatTime == arrBeatTime[i])
		{
			m_vecBeatSpr.at(i)->setVisible(false);
		}
	}


	bool bExtraFlag = (m_iBeatTime < 0);
	//����ʮλ���Ŷ���
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
	//���ø�λ���Ŷ���
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


//Beat����ִ�ж������õĻص�����
//bExtraFlag��ʾ�Ƿ��Ƕ����������������ú�ɫ��ʾ
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
	//���ÿɵ��
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
		//��ʱ3�봥���ɹ���ʧ�ܽ���
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

	//��¼���η���
	UserDefault::getInstance()->setIntegerForKey("Score", iScore);
	//ͨ��
	CDataManager::getInstance()->Pass(iScore);

	//�л���ͨ�ؽ���
	auto scene = CPassScene::CreateScene();
	Director::getInstance()->replaceScene(scene);

}

void CGameScene::LevelFail()
{
#ifdef _DEBUG_
	log("LevelFail.");
#endif

	//�л���ʧ�ܽ���
	auto scene = CFailScene::CreateScene();
	Director::getInstance()->replaceScene(scene);

}



//��ͷת����
const int arrArrowChangeList[5][6] =
{
	{ 0, 0, 0, 0, 0, 0 },
	{ 3, 2, 1, 1, 2, 3 },
	{ 2, 1, 3, 3, 1, 2 },
	{ 1, 3, 2, 2, 3, 1 },
	{ 4, 4, 4, 4, 4, 4 }
};

//��ȡ�ı�֮��ļ�ͷ���ͣ�iPos��ʾ�ǵڼ���λ�ã�iCurArrowType��ʾ��ǰ��ͷ����
int CGameScene::GetChangedArrow(int iPos, int iCurArrowType)
{
	return arrArrowChangeList[iCurArrowType][iPos];
}


//�����λ�ð�����ͷ
void CGameScene::ChangeWithArrow(int iRowIndex, int iColIndex, int iArrow)
{
	if (iArrow == HexagonArrow_Hexagon)
	{
		//�ı��Լ�
		m_arrHexagon[iRowIndex][iColIndex].Change(true);
		return;
	}

	if (iArrow == HexagonArrow_Transverse)
	{
		//һ��������
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

	//�ı��Լ�
	m_arrHexagon[iRowIndex][iColIndex].Change(true);

	if (iArrow == HexagonArrow_LeftTop)
	{
		int iTemp = ((iRowIndex % 2 == 0) ? 0 : 1);
		for (int i = iRowIndex + 1; i < ROW_MAX; i++)
		{
			//һ��������
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
			//һ��������
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
			//һ��������
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
			//һ��������
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

//�����λ��û�м�ͷ
void CGameScene::ChangeWithoutArrow(int iRowIndex, int iColIndex)
{
	//�ı��Լ�
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
		//һ��������
		int iColCount = COLUMN_MAX + ((iRowIdx % 2 == 0) ? 0 : -1);
		if (arrHexagonIdx[i].IsValid(ROW_MAX, iColCount) && m_arrHexagon[iRowIdx][iColIdx].GetValid())
		{
			int iArrowType = m_arrHexagon[iRowIdx][iColIdx].GetArrow();
			int iChangedArrowType = GetChangedArrow(i, iArrowType);
			log("[%d][%d]: iArrowType=%d->%d", iRowIdx, iColIdx, iArrowType, iChangedArrowType);
			if (iChangedArrowType == iArrowType)
			{
				//û�иı�ʱ��ΪNone
				iChangedArrowType = HexagonArrow_None;
			}
			m_arrHexagon[iRowIdx][iColIdx].Change(false, iChangedArrowType);
		}
	}
}


//��ʱ�л��ɹ���ʧ�ܽ���
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