#pragma once
#include "cocos2d.h"
class CMenuScene : public cocos2d::Layer
{
private:
	//�����ĸ���������ͼƬ
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

	//�����ť����Ӧ
	void OnButtonClick(Ref* pSender, int iIndex);

	//����ִ�н�����Ļص������ڻص�Ч��
	//bChangeXFlag��ʾ�����Ƿ�ı�X��
	//bIncreaseFlag��ʾ�����ı�ĳһ��ʱ�Ƿ���������
	void OnActionFinish(Ref* pSender, bool bChangeXFlag, bool bIncreaseFlag);

	//��������������
	void CreateKeyListener();

	CREATE_FUNC(CMenuScene);
};

