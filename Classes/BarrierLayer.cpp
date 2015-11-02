#include "BarrierLayer.h"

USING_NS_CC;

CBarrierLayer::CBarrierLayer()
{

}

CBarrierLayer::~CBarrierLayer()
{

}


bool CBarrierLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	//添加一个半灰色的层
    LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 150));
    addChild(backLayerColor);

    //添加向下触摸屏蔽
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };

    //创建监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

    return true;
}