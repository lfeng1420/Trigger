#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;



//因为关卡Sprite需要根据选择的主题变化而变化，所以写成一个类
class LevelSpr
{
public:
	cocos2d::Sprite* m_pNumDecSpr;	//十位

	cocos2d::Sprite* m_pNumUniSpr;	//个位

	cocos2d::Sprite* m_pStarSpr[3];	//小星星

	cocos2d::Sprite* m_pLockSpr;	//锁

	bool m_bLockState;				//是否锁住

public:

	LevelSpr();

	void SetDec(int iTheme, int iNum);

	void SetUni(int iTheme, int iNum);

	void SetLock(int iTheme);

	void SetLockState(bool bLock);

	cocos2d::Size GetNumSize();

	cocos2d::Size GetStarSize();

	void SetPos(bool bDoubleNum);

	void SetStar(int iScore);

	bool GetLockState();
};


class CThemeScene : public cocos2d::Layer, public ScrollViewDelegate
{
public:
	enum
	{
		Theme_Num = 10,			//主题数

		Level_Num = 20,			//关卡数

		BtnIndex_Back = 10,

		Level_Height = 146,		//关卡Sprite高度

		Theme_Height = 134,		//主题Sprite高度

		SpriteTag_LevelUnit = 1420,	//Sprite标签 - 关卡
	};

private:
	cocos2d::Layer* m_pThemeLayer;		//主题滚动层

	cocos2d::Layer* m_pLevelLayer;	//关卡滚动层

	cocos2d::extension::ScrollView* m_pThemeScrollView;	//ScrollView

	cocos2d::extension::ScrollView* m_pLevelScrollView;	//ScrollView

	cocos2d::Vector<cocos2d::Sprite *> m_themeSprVec;	//主题Sprite序列

	LevelSpr m_oLevelSpr[Level_Num];		//关卡Sprite数组

	cocos2d::Sprite* m_pBgSprite;	//背景图片

	cocos2d::MenuItemSprite* m_pBackBtn;	//返回按钮

	cocos2d::Vec2 m_themeTouchPos;	//主题界面点击的位置

	cocos2d::Vec2 m_themeOffsetPos; //主题Layer偏移位置

	cocos2d::Size m_themeSize;		//单个主题Sprite大小

	cocos2d::Vec2 m_levelOffsetPos; //关卡Layer偏移位置

	cocos2d::Vec2 m_levelTouchPos;	//关卡界面点击的位置

	cocos2d::Size m_visibleSize;	//可视区域大小

	cocos2d::Rect m_pointBoundingBox;//指针区域Rect

	int m_iSelectTheme;				//选择的主题

public:
	CThemeScene();
	~CThemeScene();

	//cocos function
	static cocos2d::Scene* CreateScene();

	//cocos function
	virtual bool init();

	CREATE_FUNC(CThemeScene);

	//初始化主题Layer
	void InitThemeLayer();

	//初始化ScrollView
	void InitScrollView();

	//初始化背景
	void InitBGAndBtn();

	//根据选择的主题设置背景
	void SetBackground();

	//初始化关卡Layer，fHeight表示关卡单个高度
	void InitLevelLayer();

	//构建关卡列表
	void BuildLevel();

	//创建触摸监听
	void CreateTouchListener();

	//创建按键监听
	void CreateKeyListener();

	//scroll delegate
	void scrollViewDidScroll(ScrollView* view){};

	void scrollViewDidZoom(ScrollView* view){};

	void scrollViewMoveOver(ScrollView* view){};

	//点击菜单项
	void OnMenuClick(Ref* pSender, int iIndex);

	//主题Sprite执行动作，bMoveOut表示是否移出
	void TakeThemeAction(bool bMoveOut);

	//Theme动作回调
	void OnThemeActionCallback(Node* pSender, bool bMoveOut);
};
