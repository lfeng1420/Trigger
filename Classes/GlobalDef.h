#pragma once
#include "cocos2d.h"
#include <algorithm>
#include <vector>

using namespace std;

//#define _DEBUG_

// 获取实际关卡序号，x为地图编号(从0开始)，y为当前关卡编号
#define GET_ACTUAL_LEVEL(x, y)  ((x) * 20 + (y))

//箭头信息
struct ArrowInfo
{
	int iX;
	int iY;
	int iType; //箭头类型
};

struct MapInfo
{
	int iColor;
	int iID;
	int iBeat;
	vector<int> vecMap;
	vector<ArrowInfo> vecMapItem;
	vector<int> vecUserItem;
};

typedef vector<MapInfo> MapVector;
typedef vector<int>::iterator VecIntIterator;

struct UserInfo
{
	int iScore;
	int iLevel;
	vector<int> vecData;
};

enum HexagonArrow
{
	HexagonArrow_None,				//无箭头
	HexagonArrow_Transverse,		//横向
	HexagonArrow_LeftTop,			//左上角
	HexagonArrow_RightTop,			//右上角
	HexagonArrow_Hexagon,			//六边形
};

enum HexagonColor
{
	HexagonColor_None,			//无
	HexagonColor_White,			//白色
	HexagonColor_Theme,			//与主题颜色相同
};

enum 
{
	TOTAL_LEVEL_NUM = 200,
	THEME_NUM = 10,
	LEVEL_NUM = 20,
	COLUMN_MAX = 7,
	ROW_MAX = 9,
	BEAT_TIME_LIMIT = -10,
	ACTION_REBOUND_LEN = 20,
	MENU_SCENE_TAG = 1420,
	LEVEL_INDEX_MAX = 19,
	THEME_INDEX_MAX = 9,
};