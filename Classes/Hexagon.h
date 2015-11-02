#pragma once
#include "cocos2d.h"
#include "GlobalDef.h"

class CHexagon : public cocos2d::Node
{
private:
	cocos2d::Node* m_pGatherNode;					//����Sprite

	cocos2d::Sprite* m_pNumSprite;					//����Sprite

	cocos2d::Sprite* m_pArrowSprite;				//��ͷSprite

	cocos2d::Sprite* m_pBgSprite;					//����Sprite

	cocos2d::Sprite* m_pEfferSprite;				//Ч��Sprite

	int m_iNum;										//����

	int m_iArrow;									//��ͷ

	int m_iColor;									//��ɫ

	bool m_bValidFlag;								//�Ƿ���Ч

public:
	CHexagon();
	~CHexagon();

	//Init����
	void Init(int iColorAndNum);

	//��ȡ��ͷ����
	int GetArrow();

	//���ü�ͷ���ͣ�bRealSet��ʾ�Ƿ���������
	//��Ϊ������ʱ��ͷֻ����Ϊ���ֵı߿򣬲�������м�ͷ
	void SetArrow(int iArrow, bool bRealSet = true);

	//��ñ�����ɫ
	int GetColor();

	//���ñ�����ɫ
	void SetColor(int iColor);

	//�������
	int GetNum();

	//��������
	void SetNum(int iNum);

	//�����Ƿ���Ч
	void SetValid(bool bValidFlag);

	//��ȡ��Ч��־
	bool GetValid();

	//��ȡ��С
	cocos2d::Size GetSize();

	//��ȡ���ϵ�Sprite
	cocos2d::Node* GetGatherNode();

	//�����ı����Ӧ
	//bClickSelfFlag��ʾ������Ƿ����Լ�
	//iArrowType��ʾҪ�ı�ɵ�ArrowType����Ϊ�����ͷ����Χ��ʹ��ͷ�������仯��Ĭ�ϲ������仯
	void Change(bool bClickSelfFlag, int iArrowType = HexagonArrow_None);

	//�����ص�
	void OnActionCallback(Node* pSender, bool bClickSelfFlag, int iArrowType);

	//��ȡ�Ƿ���׼��״̬������ǰ�����η���ͨ��Ҫ��
	bool GetReady();
};

