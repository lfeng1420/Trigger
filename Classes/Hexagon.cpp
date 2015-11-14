#include "Hexagon.h"
#include "DataManager.h"
#include "ResManager.h"

USING_NS_CC;

CHexagon::CHexagon() : m_pGatherNode(nullptr), m_pBgSprite(nullptr), m_pNumSprite(nullptr),
						m_pArrowSprite(nullptr), m_pEfferSprite(nullptr),
						m_iArrow(HexagonArrow_None), m_iColor(HexagonColor_None),
						m_iNum(0), m_bValidFlag(false)
{
}


CHexagon::~CHexagon()
{
	//CC_SAFE_RELEASE_NULL(m_pNumSprite);
	//CC_SAFE_RELEASE_NULL(m_pBgSprite);
	//CC_SAFE_RELEASE_NULL(m_pArrowSprite);
}


//Create函数
void CHexagon::Init(int iColorAndNum)
{
	m_pBgSprite = Sprite::create();
	m_pArrowSprite = Sprite::create();
	m_pNumSprite = Sprite::create();
	m_pEfferSprite = Sprite::create();

	//大于10时颜色默认为白色，数字为最后一位 - 2
	if (iColorAndNum > 10)
	{
		int iNum = iColorAndNum - iColorAndNum / 10 * 10 - 1;
		SetColor(HexagonColor_White);
		SetNum(iNum);
	}
	else if (iColorAndNum <= 10 && iColorAndNum > HexagonColor_Theme)
	{
		//大于HexagonColor_Theme且小于等于10时颜色默认是主题色，数字为iColorAndNum - 2
		SetColor(HexagonColor_Theme);
		SetNum(iColorAndNum - 2);
	}
	else
	{
		//默认是颜色
		SetColor(iColorAndNum);
	}

	//创建MenuItemSprite
	m_pGatherNode = Node::create();
	m_pGatherNode->addChild(m_pBgSprite);
	m_pGatherNode->addChild(m_pArrowSprite);
	m_pArrowSprite->setPosition(0, -1);
	m_pGatherNode->addChild(m_pNumSprite);
	m_pNumSprite->setPosition(-1, 0);
	m_pGatherNode->addChild(m_pEfferSprite);
}

//获取箭头类型
int CHexagon::GetArrow()
{
	return m_iArrow;
}

//设置箭头类型
void CHexagon::SetArrow(int iArrow, bool bRealSet)
{
	if (bRealSet)
	{
		m_iArrow = iArrow;
	}

	//无箭头
	if (iArrow == HexagonArrow_None)
	{
		m_pArrowSprite->setVisible(false);
		return;
	}

	char name[20] = {'\0'};

	//如果背景是白色，则箭头应该是主题色
	if (m_iColor == HexagonColor_White)
	{
		int iThemeColor = CDataManager::getInstance()->GetCurThemeColor();
		sprintf(name, "item_%d_%d.png", iArrow, iThemeColor);
	}
	else if (m_iColor == HexagonColor_Theme)
	{
		//如果背景是主题色，则箭头应该是白色
		sprintf(name, "item_%d_w.png", iArrow);
	}

	//设置Sprite
	m_pArrowSprite->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(name));
	m_pArrowSprite->setVisible(true);
}

//获得背景颜色
int CHexagon::GetColor()
{
	return m_iColor;
}


//设置背景颜色
void CHexagon::SetColor(int iColor)
{
	m_iColor = iColor;

	//空白六边形
	if (m_iColor == HexagonColor_None)
	{
		m_pBgSprite->setVisible(false);
		return;
	}

	//Sprite名称
	char arrBgName[20] = {'\0'};
	char arrEfferName[20] = { '\0' };
	if (m_iColor == HexagonColor_White)
	{
		sprintf(arrBgName, "block_w.png");
		sprintf(arrEfferName, "effer_w.png");
	}
	else if (m_iColor == HexagonColor_Theme)
	{
		int iThemeColor = CDataManager::getInstance()->GetCurThemeColor();
		sprintf(arrBgName, "block_%d.png", iThemeColor);
		sprintf(arrEfferName, "effer_%d.png", iThemeColor);
	}

	m_pBgSprite->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(arrBgName));
	m_pBgSprite->setVisible(true);

	//效果
	m_pEfferSprite->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(arrEfferName));
	m_pEfferSprite->setVisible(false);
}

//获得数字
int CHexagon::GetNum()
{
	return m_iNum;
}

//设置数字
void CHexagon::SetNum(int iNum)
{
	m_iNum = iNum;

	//没有数字
	if (m_iNum == 0)
	{
		m_pNumSprite->setVisible(false);
		SetArrow(HexagonArrow_None, false);
		return;
	}

	//Sprite名称
	char name[20];
	if (HexagonColor_White == m_iColor)
	{
		int iThemeColor = CDataManager::getInstance()->GetCurThemeColor();
		sprintf(name, "number_%d_%d.png", iThemeColor, m_iNum);
	}
	else if (HexagonColor_Theme == m_iColor)
	{
		sprintf(name, "number_w_%d.png", m_iNum);
	}
	
	m_pNumSprite->setScale(0.5f);
	m_pNumSprite->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(name));
	m_pNumSprite->setVisible(true);
	SetArrow(HexagonArrow_Hexagon, false);
}


//取得集合Sprite的Node
Node* CHexagon::GetGatherNode()
{
	return m_pGatherNode;
}


void CHexagon::Change(bool bClickSelfFlag, int iArrowType)
{
	//有数字则直接递减即可
	if (m_iNum > 0)
	{
		SetNum(m_iNum - 1);
		return;
	}

	//缩放+淡出
	auto efferScaleBy = ScaleBy::create(0.13f, 5.0f);
	auto effScaleBack = efferScaleBy->reverse();
	auto efferFadeOut = FadeOut::create(0.13f);
	//设置透明度
	m_pEfferSprite->setOpacity(255);
	m_pEfferSprite->setVisible(true);
	//执行动作
	m_pEfferSprite->runAction(
		Sequence::create(
			Spawn::createWithTwoActions(efferScaleBy, efferFadeOut),
			effScaleBack,
			NULL
		)
	);

	auto gatherScaleBy = ScaleBy::create(0.13f, 0.05f);
	auto gatherScaleBack = gatherScaleBy->reverse();
	m_pGatherNode->runAction(
		Sequence::create(
			gatherScaleBy,
			CallFuncN::create(CC_CALLBACK_1(CHexagon::OnActionCallback, this, bClickSelfFlag, iArrowType)),
			gatherScaleBack,
			NULL
		)
	);
}


//获取大小
Size CHexagon::GetSize()
{
	return m_pBgSprite->getContentSize();
}



//设置是否有效
void CHexagon::SetValid(bool bValidFlag)
{
	m_bValidFlag = bValidFlag;
}

//获取有效标志
bool CHexagon::GetValid()
{
	return m_bValidFlag;
}


//动作回调
void CHexagon::OnActionCallback(Node* pSender, bool bClickSelfFlag, int iArrowType)
{
	//主题色和白色之间切换
	if (m_iColor == HexagonColor_Theme)
	{
		SetColor(HexagonColor_White);
	}
	else
	{
		SetColor(HexagonColor_Theme);
	}

	int iArrow = m_iArrow;
	//箭头需要点击才会消除
	if (bClickSelfFlag && m_iArrow > HexagonArrow_None)
	{
		iArrow = HexagonArrow_None;
	}
	else if (!bClickSelfFlag && iArrowType > HexagonArrow_None)
	{
		//箭头受到了影响
		iArrow = iArrowType;
	}
	SetArrow(iArrow, true);

	//隐藏效果
	m_pEfferSprite->setVisible(false);
}


//获取是否已准备状态，即当前六边形符合通关要求
bool CHexagon::GetReady()
{
	if (m_iArrow == HexagonArrow_None && m_iNum == 0 && m_iColor == HexagonColor_White)
	{
		return true;
	}

	return false;
}
