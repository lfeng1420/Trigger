#pragma once
#include "GlobalDef.h"

class CResManager
{
private:
	cocos2d::SpriteFrameCache* m_frameCache;

	cocos2d::AnimationCache* m_animCache;

	CocosDenshion::SimpleAudioEngine* m_audioCache;

	bool m_bSoundState;

public:
	CResManager();
	~CResManager();

	static CResManager* getInstance();

	//��ʼ��
	bool Init();

	//���ݸ��������·�����ļ���׺�������ļ������������ļ��У�
	void SearchFile(const char* pDir, const char* pFileExtension);

	//����plist
	void LoadPlist(const char* pName);

	//�Ƴ��Ѿ����ص�Plist
	void RemovePlist(const char* pName);

	//��ȡSpriteFrame
	cocos2d::SpriteFrame* GetSpriteFrame(char* pName);
	
	//��Ӷ���
	void AddAnim(char* pName[], int iLen, float fDelay, char* pAnimName);

	//��ȡ����
	cocos2d::Animate* GetAnim(char* pName);

	//Ԥ������Ч
	void PreloadSound(const char* pName);

	//������Ч
	void PlayEffect(const char* pName);

	//��������״̬Ϊָ��״̬
	void SetSoundState(bool bSoundState);
	
	//��ȡ����״̬
	bool GetSoundState();

	//��������״̬
	void SaveSoundState();
};

