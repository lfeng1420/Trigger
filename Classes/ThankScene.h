#pragma once
#include "GlobalDef.h"
class CThankScene : public cocos2d::Layer
{
private:
	cocos2d::Sprite* m_pInfoSprite;

	cocos2d::Sprite* m_pLogoSprite;

	cocos2d::Size m_visibleSize;

public:
	CThankScene();
	~CThankScene();

	//cocos function
	static cocos2d::Scene* CreateScene();

	//cocos function
	virtual bool init();

	//创建按键监听
	void CreateKeyListener();

	//创建触摸监听
	void CreateTouchListener();

	//执行动作
	void TakeAction();

	//返回菜单界面
	void ReturnMenu(Node* pSender);

	CREATE_FUNC(CThankScene);
};

