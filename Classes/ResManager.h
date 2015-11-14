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

	//初始化
	bool Init();

	//根据给定的相对路径和文件后缀名搜索文件（不包括子文件夹）
	void SearchFile(const char* pDir, const char* pFileExtension);

	//加载plist
	void LoadPlist(const char* pName);

	//移除已经加载的Plist
	void RemovePlist(const char* pName);

	//获取SpriteFrame
	cocos2d::SpriteFrame* GetSpriteFrame(char* pName);
	
	//添加动画
	void AddAnim(char* pName[], int iLen, float fDelay, char* pAnimName);

	//获取动画
	cocos2d::Animate* GetAnim(char* pName);

	//预加载声效
	void PreloadSound(const char* pName);

	//播放音效
	void PlayEffect(const char* pName);

	//设置声音状态为指定状态
	void SetSoundState(bool bSoundState);
	
	//获取声音状态
	bool GetSoundState();

	//保存声音状态
	void SaveSoundState();
};

