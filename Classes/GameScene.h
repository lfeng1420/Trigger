#pragma once
#include "cocos2d.h"
#include "GlobalDef.h"
#include "Hexagon.h"


class UserItemSpr
{
private:
	int m_iType;					//��ͷ����

	bool m_bValid;					//�Ƿ���Ч

public:
	cocos2d::Sprite* m_pArrowSpr;	//��ͷSprite

	cocos2d::Sprite* m_pClickSpr;	//���ʱ��ӵĿ�Sprite

	UserItemSpr() : m_pArrowSpr(nullptr), m_pClickSpr(nullptr),
					m_iType(0), m_bValid(false)
	{

	}

	//�Ƿ���Ч
	bool IsValid()
	{
		return m_bValid;
	}

	//��ȡ��ͷ����
	int GetArrowType()
	{
		return m_iType;
	}

	//���ü�ͷ����
	void SetArrowType(int iType)
	{
		m_iType = iType;
	}

	//���
	void OnClicked()
	{
		m_pClickSpr->setVisible(!IsClicked());
	}

	//ȡ�����
	void OnUnclicked()
	{
		m_pClickSpr->setVisible(false);
	}

	//�Ƿ񱻵��
	bool IsClicked()
	{
		return m_pClickSpr->isVisible();
	}

	//��ʹ��
	void OnUsed()
	{
		m_pArrowSpr->setVisible(false);
		m_pClickSpr->setVisible(false);
		m_bValid = false;
	}

	//�����Ƿ���Ч
	void SetValid(bool bValid)
	{
		m_bValid = bValid;
	}
};




class CGameScene : public cocos2d::LayerColor
{
private:
	enum
	{
		COLUMN_MAX = 7,
		ROW_MAX = 9,
		EVERY_THEME_MAP_MAX = 20,
		BEAT_TIME_LIMIT = -10
	};

	//����������
	CHexagon m_arrHexagon[ROW_MAX][COLUMN_MAX];

	int m_iValidRowStart;	//��Ч��ʼ����������ͼ���������ļ�����ɵģ�������Ҫ������ʼ����������

	int m_iValidRowNum;		//��Ч����

	int m_iCurTheme;		//��ǰ����

	int m_iCurLevel;		//��ǰ�ؿ�

	bool m_bGameState;		//��Ϸ״̬

	int m_iBeatTime;		//Beat������С��0ʱ��ʾ�ؿ�Ԥ��Beat����

	cocos2d::Vec2 m_fGatherOffset;	//����hexagon���е�����ƫ��

	cocos2d::Vector<cocos2d::Sprite*> m_vecBeatSpr;	//Beat Spriteͼʾ����

	int m_iClickUserItemIndex;	//��ǰ�����UserItem����

	UserItemSpr m_oUserItem[5];	//UserItem���ϣ����5��

	cocos2d::Sprite* m_pBeatDecadeSpr;//Beat����ʮλSprite

	cocos2d::Sprite* m_pBeatUnitSpr;//Beat������λSprite

	cocos2d::Size m_visibleSize;	//���������С

	bool m_bCanClicked;				//�ɵ����־

public:
	CGameScene();
	~CGameScene();

	//cocos function
	static cocos2d::Scene* CreateScene();

	//cocos function
	virtual bool init();

	//���ùؿ�
	void SetupLevel();

	//��������
	void CreateBackground();

	//��ʼ����ʾ�ؿ���Ϣ
	float InitLevelInfo();

	//��ʼ����ʾBeat������Ϣ
	float InitBeatInfo(float fTopHeight);

	//��ʼ����ͼ
	void InitHexagon(float fTopHeight, float fBottomHeight);

	//��ʼ���ײ�UserItem�˵�
	float CreateUserItemMenu();

	//��������������
	void CreateTouchListener();

	//������ĸ������α����
	bool CheckHexagonClick(cocos2d::Vec2 touchPos);

	//�����ε������
	void OnHexagonClick(int iRowIndex, int iColIndex);

	//�����ť��Ӧ
	void OnButtonClick(Ref* pSender, int iIndex);

	//UserItem�����Ӧ
	void OnUserItemClick(Ref* pSender, int iIndex);

	//Beat�����ı�
	void ChangeBeatTime();

	//Beat����ִ�ж������õĻص�������bExtraFlag��ʾ�Ƿ��Ƕ����������������ú�ɫ��ʾ
	void OnBeatActionCallback(Node* pSender, bool bExtraFlag, int iNum);

	//�����Ϸ�Ƿ����
	void CheckGameOver(float dt);

	//ͨ��
	void LevelPass();

	//ʧ��
	void LevelFail();
	
	//��ȡ�ı�֮��ļ�ͷ���ͣ�iPos��ʾ�ǵڼ���λ�ã�iCurArrowType��ʾ��ǰ��ͷ����
	int GetChangedArrow(int iPos, int iCurArrowType);

	//�����λ�ð�����ͷ
	void ChangeWithArrow(int iRowIndex, int iColIndex, int iArrow);

	//�����λ��û�м�ͷ
	void ChangeWithoutArrow(int iRowIndex, int iColIndex);

	//��ȡ���������δ�С
	cocos2d::Size GetHexagonSize();

	//��ʱ�л��ɹ���ʧ�ܽ���
	void OnTimeCallback(Node* pSender, bool bPassFlag);

	CREATE_FUNC(CGameScene);
};
