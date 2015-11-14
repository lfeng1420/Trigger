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

	//������������
	void CreateKeyListener();

	//������������
	void CreateTouchListener();

	//ִ�ж���
	void TakeAction();

	//���ز˵�����
	void ReturnMenu(Node* pSender);

	CREATE_FUNC(CThankScene);
};

