#pragma once

#include "GlobalDef.h"

class CPassScene : public cocos2d::LayerColor
{
private:
	int m_iCurLevel;	//当前关卡

	int m_iCurTheme;	//当前主题

	cocos2d::Size m_visibleSize;	//可视区域大小

public:
	CPassScene();
	~CPassScene();

	//cocos function
	static cocos2d::Scene* CreateScene();

	//cocos function
	virtual bool init();

	//背景
	void CreateBackground();

	//星星，即分数
	void ShowStar();

	//菜单
	void CreateMenu();

	//设置下一关
	void SetNextLevel();

	//菜单响应
	void OnMenuCallback(cocos2d::Ref* pSender, int iIndex);

	CREATE_FUNC(CPassScene);
};

