#pragma once

#include "cocos2d.h"
#include "GlobalDef.h"

class CDataManager
{
private:
	MapVector m_oMapVec;			//地图数据

	UserInfo m_stUserInfo;			//用户数据

	cocos2d::Size m_visibleSize;

	cocos2d::Vec2 m_vecOrigin;

	bool m_bSoundState;				//声音状态

	int m_iCurTheme;				//当前主题

	int m_iCurLevel;				//当前关卡

private:
	//移除string中所有位置出现过的某个字符
	void SplitString(string &srcStr, char chTarget, int* pArray);

public:
	CDataManager();
	~CDataManager();

	//获取单例
	static CDataManager* getInstance();

	//加载地图数据，从json中读取
	bool LoadMapData();

	//创建用户数据文件并初始化
	void InitUserData(const char* pName);

	//加载用户数据，从json中读取
	bool LoadUserData();

	//用户数据写入到指定文件中
	bool SaveUserData();

	//获得地图数据
	const MapInfo* GetMapInfo();

	//通过当前关卡成功
	void Pass(int iScore);

	//初始化
	bool Init();

	//设置当前主题和关卡
	void SetCurThemeAndLevel(int iCurTheme, int iCurLevel);

	//获得当前主题颜色
	int GetCurThemeColor();

	//获取未通过的关卡
	int GetUnpassLevel();

	//获取主题对应分数
	int GetThemeScore(int iTheme);

	//获得关卡的分数
	int GetLevelScore(int iTheme, int iLevel);

	//获得总分数
	int GetTotalScore();
};

