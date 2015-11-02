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



//Create����
void CHexagon::Init(int iColorAndNum)
{
	m_pBgSprite = Sprite::create();
	m_pArrowSprite = Sprite::create();
	m_pNumSprite = Sprite::create();
	m_pEfferSprite = Sprite::create();

	//����10ʱ��ɫĬ��Ϊ��ɫ������Ϊ���һλ - 2
	if (iColorAndNum > 10)
	{
		int iNum = iColorAndNum - iColorAndNum / 10 * 10 - 1;
		SetColor(HexagonColor_White);
		SetNum(iNum);
	}
	else if (iColorAndNum <= 10 && iColorAndNum > HexagonColor_Theme)
	{
		//����HexagonColor_Theme��С�ڵ���10ʱ��ɫĬ��������ɫ������ΪiColorAndNum - 2
		SetColor(HexagonColor_Theme);
		SetNum(iColorAndNum - 2);
	}
	else
	{
		//Ĭ������ɫ
		SetColor(iColorAndNum);
	}

	//����MenuItemSprite
	m_pGatherNode = Node::create();
	m_pGatherNode->addChild(m_pBgSprite);
	m_pGatherNode->addChild(m_pArrowSprite);
	m_pArrowSprite->setPosition(0, -1);
	m_pGatherNode->addChild(m_pNumSprite);
	m_pNumSprite->setPosition(-1, 0);
	m_pGatherNode->addChild(m_pEfferSprite);
}

//��ȡ��ͷ����
int CHexagon::GetArrow()
{
	return m_iArrow;
}

//���ü�ͷ����
void CHexagon::SetArrow(int iArrow, bool bRealSet)
{
	if (bRealSet)
	{
		m_iArrow = iArrow;
	}

	//�޼�ͷ
	if (m_iArrow == HexagonArrow_None)
	{
		m_pArrowSprite->setVisible(false);
		return;
	}

	char name[20] = {'\0'};

	//��������ǰ�ɫ�����ͷӦ��������ɫ
	if (m_iColor == HexagonColor_White)
	{
		int iThemeColor = CDataManager::getInstance()->GetCurThemeColor();
		sprintf(name, "item_%d_%d.png", m_iArrow, iThemeColor);
	}
	else if (m_iColor == HexagonColor_Theme)
	{
		//�������������ɫ�����ͷӦ���ǰ�ɫ
		sprintf(name, "item_%d_w.png", m_iArrow);
	}

	//����Sprite
	m_pArrowSprite->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(name));
	m_pArrowSprite->setVisible(true);
}

//��ñ�����ɫ
int CHexagon::GetColor()
{
	return m_iColor;
}


//���ñ�����ɫ
void CHexagon::SetColor(int iColor)
{
	m_iColor = iColor;

	//�հ�������
	if (m_iColor == HexagonColor_None)
	{
		m_pBgSprite->setVisible(false);
		return;
	}

	//Sprite����
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

	//Ч��
	m_pEfferSprite->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(arrEfferName));
	m_pEfferSprite->setVisible(false);
}

//�������
int CHexagon::GetNum()
{
	return m_iNum;
}

//��������
void CHexagon::SetNum(int iNum)
{
	m_iNum = iNum;

	//û������
	if (m_iNum == 0)
	{
		m_pNumSprite->setVisible(false);
		SetArrow(HexagonArrow_None, false);
		return;
	}

	//Sprite����
	char name[20];
	int iThemeColor = CDataManager::getInstance()->GetCurThemeColor();
	sprintf(name, "number_%d_%d.png", iThemeColor, m_iNum);
	
	m_pNumSprite->setSpriteFrame(CResManager::getInstance()->GetSpriteFrame(name));
	m_pNumSprite->setScale(0.5f);
	m_pNumSprite->setVisible(true);
	SetArrow(HexagonArrow_Hexagon, false);
}


//ȡ�ü���Sprite��Node
Node* CHexagon::GetGatherNode()
{
	return m_pGatherNode;
}


void CHexagon::Change(bool bClickSelfFlag, int iArrowType)
{
	//��������ֱ�ӵݼ�����
	if (m_iNum > 0)
	{
		SetNum(m_iNum - 1);
		return;
	}

	//����+����
	auto efferScaleBy = ScaleBy::create(0.1f, 5.0f);
	auto effScaleBack = efferScaleBy->reverse();
	auto efferFadeOut = FadeOut::create(0.1f);
	//����͸����
	m_pEfferSprite->setOpacity(255);
	m_pEfferSprite->setVisible(true);
	//ִ�ж���
	m_pEfferSprite->runAction(
		Sequence::create(
			Spawn::createWithTwoActions(efferScaleBy, efferFadeOut),
			effScaleBack,
			NULL
		)
	);

	auto gatherScaleBy = ScaleBy::create(0.1f, 0.05f);
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


//��ȡ��С
Size CHexagon::GetSize()
{
	return m_pBgSprite->getContentSize();
}



//�����Ƿ���Ч
void CHexagon::SetValid(bool bValidFlag)
{
	m_bValidFlag = bValidFlag;
}

//��ȡ��Ч��־
bool CHexagon::GetValid()
{
	return m_bValidFlag;
}


//�����ص�
void CHexagon::OnActionCallback(Node* pSender, bool bClickSelfFlag, int iArrowType)
{
	//����ɫ�Ͱ�ɫ֮���л�
	if (m_iColor == HexagonColor_Theme)
	{
		SetColor(HexagonColor_White);
	}
	else
	{
		SetColor(HexagonColor_Theme);
	}

	int iArrow = m_iArrow;
	//��ͷ��Ҫ����Ż�����
	if (bClickSelfFlag && m_iArrow > HexagonArrow_None)
	{
		iArrow = HexagonArrow_None;
	}
	else if (!bClickSelfFlag && iArrowType > HexagonArrow_None)
	{
		//��ͷ�ܵ���Ӱ��
		iArrow = iArrowType;
	}
	SetArrow(iArrow, true);

	//����Ч��
	m_pEfferSprite->setVisible(false);
}


//��ȡ�Ƿ���׼��״̬������ǰ�����η���ͨ��Ҫ��
bool CHexagon::GetReady()
{
	if (m_iArrow == HexagonArrow_None && m_iNum == 0 && m_iColor == HexagonColor_White)
	{
		return true;
	}

	return false;
}