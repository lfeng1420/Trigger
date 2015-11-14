#pragma once
#include "cocos2d.h"
class CMenuScene : public cocos2d::Layer
{
private:
	//保存四个声音开关图片
	cocos2d::Sprite* m_soundOnSprite1;
	cocos2d::Sprite* m_soundOnSprite2;
	cocos2d::Sprite* m_soundOffSprite1;
	cocos2d::Sprite* m_soundOffSprite2;

public:
	CMenuScene();
	~CMenuScene();

	//cocos function
	static cocos2d::Scene* CreateScene();

	//cocos function
	virtual bool init();

	//点击按钮的响应
	void OnButtonClick(Ref* pSender, int iIndex);

	//动画执行结束后的回调，用于回弹效果
	//bChangeXFlag表示动画是否改变X轴
	//bIncreaseFlag表示动画改变某一轴时是否是增长的
	void OnActionFinish(Ref* pSender, bool bChangeXFlag, bool bIncreaseFlag);

	//创建按键监听器
	void CreateKeyListener();

	CREATE_FUNC(CMenuScene);
};

