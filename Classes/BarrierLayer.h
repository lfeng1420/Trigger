#pragma once
#include "GlobalDef.h"

class CBarrierLayer : public cocos2d::Layer
{
public:
	CBarrierLayer();
	~CBarrierLayer();

	virtual bool init();

	CREATE_FUNC(CBarrierLayer);
};