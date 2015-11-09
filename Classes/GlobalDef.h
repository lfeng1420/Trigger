#pragma once
#include <algorithm>
#include <vector>

using namespace std;

#define _DEBUG_

// ��ȡʵ�ʹؿ���ţ�xΪ��ͼ���(��0��ʼ)��yΪ��ǰ�ؿ����
#define GET_ACTUAL_LEVEL(x, y)  ((x) * 20 + (y))

//��ͷ��Ϣ
struct ArrowInfo
{
	int iX;
	int iY;
	int iType; //��ͷ����
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
	HexagonArrow_None,				//�޼�ͷ
	HexagonArrow_Transverse,		//����
	HexagonArrow_LeftTop,			//���Ͻ�
	HexagonArrow_RightTop,			//���Ͻ�
	HexagonArrow_Hexagon,			//������
};

enum HexagonColor
{
	HexagonColor_None,			//��
	HexagonColor_White,			//��ɫ
	HexagonColor_Theme,			//��������ɫ��ͬ
};

