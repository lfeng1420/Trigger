#pragma once

#include "cocos2d.h"
#include "GlobalDef.h"

class CDataManager
{
private:
	MapVector m_oMapVec;			//��ͼ����

	UserInfo m_stUserInfo;			//�û�����

	cocos2d::Size m_visibleSize;

	cocos2d::Vec2 m_vecOrigin;

	bool m_bSoundState;				//����״̬

	int m_iCurTheme;				//��ǰ����

	int m_iCurLevel;				//��ǰ�ؿ�

private:
	//�Ƴ�string������λ�ó��ֹ���ĳ���ַ�
	void SplitString(string &srcStr, char chTarget, int* pArray);

public:
	CDataManager();
	~CDataManager();

	//��ȡ����
	static CDataManager* getInstance();

	//���ص�ͼ���ݣ���json�ж�ȡ
	bool LoadMapData();

	//�����û������ļ�����ʼ��
	void InitUserData(const char* pName);

	//�����û����ݣ���json�ж�ȡ
	bool LoadUserData();

	//�û�����д�뵽ָ���ļ���
	bool SaveUserData();

	//��õ�ͼ����
	const MapInfo* GetMapInfo();

	//ͨ����ǰ�ؿ��ɹ�
	void Pass(int iScore);

	//��ʼ��
	bool Init();

	//���õ�ǰ����͹ؿ�
	void SetCurThemeAndLevel(int iCurTheme, int iCurLevel);

	//��õ�ǰ������ɫ
	int GetCurThemeColor();

	//��ȡδͨ���Ĺؿ�
	int GetUnpassLevel();

	//��ȡ�����Ӧ����
	int GetThemeScore(int iTheme);

	//��ùؿ��ķ���
	int GetLevelScore(int iTheme, int iLevel);

	//����ܷ���
	int GetTotalScore();
};

