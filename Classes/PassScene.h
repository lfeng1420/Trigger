#pragma once

#include "cocos2d.h"

class CPassScene : public cocos2d::LayerColor
{
private:
	enum
	{
		LEVEL_INDEX_MAX = 19,
		THEME_INDEX_MAX = 9,
	};

	int m_iCurLevel;	//��ǰ�ؿ�

	int m_iCurTheme;	//��ǰ����

	cocos2d::Size m_visibleSize;	//���������С

public:
	CPassScene();
	~CPassScene();

	//cocos function
	static cocos2d::Scene* CreateScene();

	//cocos function
	virtual bool init();

	//����
	void CreateBackground();

	//���ǣ�������
	void ShowStar();

	//�˵�
	void CreateMenu();

	//������һ��
	void SetNextLevel();

	//�˵���Ӧ
	void OnMenuCallback(cocos2d::Ref* pSender, int iIndex);

	CREATE_FUNC(CPassScene);
};

