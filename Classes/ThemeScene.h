#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;



//��Ϊ�ؿ�Sprite��Ҫ����ѡ�������仯���仯������д��һ����
class LevelSpr
{
public:
	cocos2d::Sprite* m_pNumDecSpr;	//ʮλ

	cocos2d::Sprite* m_pNumUniSpr;	//��λ

	cocos2d::Sprite* m_pStarSpr[3];	//С����

	cocos2d::Sprite* m_pLockSpr;	//��

	bool m_bLockState;				//�Ƿ���ס

public:

	LevelSpr();

	void SetDec(int iTheme, int iNum);

	void SetUni(int iTheme, int iNum);

	void SetLock(int iTheme);

	void SetLockState(bool bLock);

	cocos2d::Size GetNumSize();

	cocos2d::Size GetStarSize();

	void SetPos(bool bDoubleNum);

	void SetStar(int iScore);

	bool GetLockState();
};


class CThemeScene : public cocos2d::Layer, public ScrollViewDelegate
{
public:
	enum
	{
		Theme_Num = 10,			//������

		Level_Num = 20,			//�ؿ���

		BtnIndex_Back = 10,

		Level_Height = 146,		//�ؿ�Sprite�߶�

		Theme_Height = 134,		//����Sprite�߶�

		SpriteTag_LevelUnit = 1420,	//Sprite��ǩ - �ؿ�
	};

private:
	cocos2d::Layer* m_pThemeLayer;		//���������

	cocos2d::Layer* m_pLevelLayer;	//�ؿ�������

	cocos2d::extension::ScrollView* m_pThemeScrollView;	//ScrollView

	cocos2d::extension::ScrollView* m_pLevelScrollView;	//ScrollView

	cocos2d::Vector<cocos2d::Sprite *> m_themeSprVec;	//����Sprite����

	LevelSpr m_oLevelSpr[Level_Num];		//�ؿ�Sprite����

	cocos2d::Sprite* m_pBgSprite;	//����ͼƬ

	cocos2d::MenuItemSprite* m_pBackBtn;	//���ذ�ť

	cocos2d::Vec2 m_themeTouchPos;	//�����������λ��

	cocos2d::Vec2 m_themeOffsetPos; //����Layerƫ��λ��

	cocos2d::Size m_themeSize;		//��������Sprite��С

	cocos2d::Vec2 m_levelOffsetPos; //�ؿ�Layerƫ��λ��

	cocos2d::Vec2 m_levelTouchPos;	//�ؿ���������λ��

	cocos2d::Size m_visibleSize;	//���������С

	cocos2d::Rect m_pointBoundingBox;//ָ������Rect

	int m_iSelectTheme;				//ѡ�������

public:
	CThemeScene();
	~CThemeScene();

	//cocos function
	static cocos2d::Scene* CreateScene();

	//cocos function
	virtual bool init();

	CREATE_FUNC(CThemeScene);

	//��ʼ������Layer
	void InitThemeLayer();

	//��ʼ��ScrollView
	void InitScrollView();

	//��ʼ������
	void InitBGAndBtn();

	//����ѡ����������ñ���
	void SetBackground();

	//��ʼ���ؿ�Layer��fHeight��ʾ�ؿ������߶�
	void InitLevelLayer();

	//�����ؿ��б�
	void BuildLevel();

	//������������
	void CreateTouchListener();

	//������������
	void CreateKeyListener();

	//scroll delegate
	void scrollViewDidScroll(ScrollView* view){};

	void scrollViewDidZoom(ScrollView* view){};

	void scrollViewMoveOver(ScrollView* view){};

	//����˵���
	void OnMenuClick(Ref* pSender, int iIndex);

	//����Spriteִ�ж�����bMoveOut��ʾ�Ƿ��Ƴ�
	void TakeThemeAction(bool bMoveOut);

	//Theme�����ص�
	void OnThemeActionCallback(Node* pSender, bool bMoveOut);
};
