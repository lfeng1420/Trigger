#pragma once

#include "cocos2d.h"

class CFailScene : public cocos2d::LayerColor
{
public:
	CFailScene();
	~CFailScene();

	//cocos function
	static cocos2d::Scene* CreateScene();

	//cocos function
	virtual bool init();

	//�˵���Ӧ
	void OnMenuCallback(cocos2d::Ref* pSender, int iIndex);


	CREATE_FUNC(CFailScene);
};

