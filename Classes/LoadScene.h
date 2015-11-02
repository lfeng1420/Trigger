#pragma once
#include "cocos2d.h"

class CLoadScene : public cocos2d::Layer
{
public:
	CLoadScene();
	~CLoadScene();

	//cocos function
	static cocos2d::Scene* CreateScene();

	//cocos function
	virtual bool init();

	//定时器调用
	void OnSwitchScene(float dt);

	CREATE_FUNC(CLoadScene);
};

