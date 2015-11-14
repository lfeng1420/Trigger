#pragma once
#include "GlobalDef.h"

class CPauseScene : public cocos2d::Layer
{
public:
	CPauseScene();
	~CPauseScene();

	virtual bool init();

	void OnMenuCallback(Ref* pSender, int iIndex);

	CREATE_FUNC(CPauseScene);
};