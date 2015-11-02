#pragma once
#include "cocos2d.h"

class CPauseScene : public cocos2d::Layer
{
public:
	CPauseScene();
	~CPauseScene();

	virtual bool init();

	void OnMenuCallback(Ref* pSender, int iIndex);

	CREATE_FUNC(CPauseScene);
};