#pragma once
#include "cocos2d.h"
#include "GlobalDef.h"
#include "Hexagon.h"


class UserItemSpr
{
private:
	int m_iType;					//箭头类型

	bool m_bValid;					//是否有效

public:
	cocos2d::Sprite* m_pArrowSpr;	//箭头Sprite

	cocos2d::Sprite* m_pClickSpr;	//点击时添加的框Sprite

	UserItemSpr() : m_pArrowSpr(nullptr), m_pClickSpr(nullptr),
					m_iType(0), m_bValid(false)
	{

	}

	//是否有效
	bool IsValid()
	{
		return m_bValid;
	}

	//获取箭头类型
	int GetArrowType()
	{
		return m_iType;
	}

	//设置箭头类型
	void SetArrowType(int iType)
	{
		m_iType = iType;
	}

	//点击
	void OnClicked()
	{
		m_pClickSpr->setVisible(!IsClicked());
	}

	//取消点击
	void OnUnclicked()
	{
		m_pClickSpr->setVisible(false);
	}

	//是否被点击
	bool IsClicked()
	{
		return m_pClickSpr->isVisible();
	}

	//已使用
	void OnUsed()
	{
		m_pArrowSpr->setVisible(false);
		m_pClickSpr->setVisible(false);
		m_bValid = false;
	}

	//设置是否有效
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

	//六边形序列
	CHexagon m_arrHexagon[ROW_MAX][COLUMN_MAX];

	int m_iValidRowStart;	//有效起始行索引，地图都是连续的几行组成的，所以需要保存起始索引和数量

	int m_iValidRowNum;		//有效行数

	int m_iCurTheme;		//当前主题

	int m_iCurLevel;		//当前关卡

	bool m_bGameState;		//游戏状态

	int m_iBeatTime;		//Beat次数，小于0时表示关卡预设Beat次数

	cocos2d::Vec2 m_fGatherOffset;	//集合hexagon居中调整的偏移

	cocos2d::Vector<cocos2d::Sprite*> m_vecBeatSpr;	//Beat Sprite图示次数

	int m_iClickUserItemIndex;	//当前点击的UserItem索引

	UserItemSpr m_oUserItem[5];	//UserItem集合，最多5个

	cocos2d::Sprite* m_pBeatDecadeSpr;//Beat次数十位Sprite

	cocos2d::Sprite* m_pBeatUnitSpr;//Beat次数个位Sprite

	cocos2d::Size m_visibleSize;	//可视区域大小

	bool m_bCanClicked;				//可点击标志

public:
	CGameScene();
	~CGameScene();

	//cocos function
	static cocos2d::Scene* CreateScene();

	//cocos function
	virtual bool init();

	//设置关卡
	void SetupLevel();

	//创建背景
	void CreateBackground();

	//初始化显示关卡信息
	float InitLevelInfo();

	//初始化显示Beat次数信息
	float InitBeatInfo(float fTopHeight);

	//初始化地图
	void InitHexagon(float fTopHeight, float fBottomHeight);

	//初始化底部UserItem菜单
	float CreateUserItemMenu();

	//创建触摸监听器
	void CreateTouchListener();

	//检查是哪个六边形被点击
	bool CheckHexagonClick(cocos2d::Vec2 touchPos);

	//六边形点击处理
	void OnHexagonClick(int iRowIndex, int iColIndex);

	//点击按钮响应
	void OnButtonClick(Ref* pSender, int iIndex);

	//UserItem点击响应
	void OnUserItemClick(Ref* pSender, int iIndex);

	//Beat次数改变
	void ChangeBeatTime();

	//Beat次数执行动作调用的回调函数，bExtraFlag表示是否是额外次数，额外次数用红色表示
	void OnBeatActionCallback(Node* pSender, bool bExtraFlag, int iNum);

	//检查游戏是否结束
	void CheckGameOver(float dt);

	//通关
	void LevelPass();

	//失败
	void LevelFail();
	
	//获取改变之后的箭头类型，iPos表示是第几个位置，iCurArrowType表示当前箭头类型
	int GetChangedArrow(int iPos, int iCurArrowType);

	//点击的位置包含箭头
	void ChangeWithArrow(int iRowIndex, int iColIndex, int iArrow);

	//点击的位置没有箭头
	void ChangeWithoutArrow(int iRowIndex, int iColIndex);

	//获取单个六边形大小
	cocos2d::Size GetHexagonSize();

	//定时切换成功或失败界面
	void OnTimeCallback(Node* pSender, bool bPassFlag);

	CREATE_FUNC(CGameScene);
};
