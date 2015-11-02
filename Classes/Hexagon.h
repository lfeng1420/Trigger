#pragma once
#include "cocos2d.h"
#include "GlobalDef.h"

class CHexagon : public cocos2d::Node
{
private:
	cocos2d::Node* m_pGatherNode;					//集合Sprite

	cocos2d::Sprite* m_pNumSprite;					//数字Sprite

	cocos2d::Sprite* m_pArrowSprite;				//箭头Sprite

	cocos2d::Sprite* m_pBgSprite;					//背景Sprite

	cocos2d::Sprite* m_pEfferSprite;				//效果Sprite

	int m_iNum;										//数字

	int m_iArrow;									//箭头

	int m_iColor;									//颜色

	bool m_bValidFlag;								//是否有效

public:
	CHexagon();
	~CHexagon();

	//Init函数
	void Init(int iColorAndNum);

	//获取箭头类型
	int GetArrow();

	//设置箭头类型，bRealSet表示是否真正设置
	//因为有数字时箭头只是作为数字的边框，不是真的有箭头
	void SetArrow(int iArrow, bool bRealSet = true);

	//获得背景颜色
	int GetColor();

	//设置背景颜色
	void SetColor(int iColor);

	//获得数字
	int GetNum();

	//设置数字
	void SetNum(int iNum);

	//设置是否有效
	void SetValid(bool bValidFlag);

	//获取有效标志
	bool GetValid();

	//获取大小
	cocos2d::Size GetSize();

	//获取集合的Sprite
	cocos2d::Node* GetGatherNode();

	//发生改变的响应
	//bClickSelfFlag表示点击的是否是自己
	//iArrowType表示要改变成的ArrowType，因为点击箭头的周围会使箭头方向发生变化。默认不发生变化
	void Change(bool bClickSelfFlag, int iArrowType = HexagonArrow_None);

	//动作回调
	void OnActionCallback(Node* pSender, bool bClickSelfFlag, int iArrowType);

	//获取是否已准备状态，即当前六边形符合通关要求
	bool GetReady();
};

