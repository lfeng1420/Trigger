#pragma once
#include "cocos2d.h"

class CBarrierLayer : public cocos2d::Layer
{
public:
	CBarrierLayer();
	~CBarrierLayer();

	virtual bool init();

	CREATE_FUNC(CBarrierLayer);
};