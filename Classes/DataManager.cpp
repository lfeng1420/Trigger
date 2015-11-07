#include "DataManager.h"
#include "json/rapidjson.h"
#include "json/writer.h"
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"

USING_NS_CC;

const char* MAP_FILE_PATH = "maps/levelmap.json";
const char* SAVE_DATA_PATH = "/LocalStorage.json";

//�ؿ�������200
const int LEVELNUM = 200;

//����ģʽ
static CDataManager* _dataManager = nullptr;

CDataManager::CDataManager() : m_bSoundState(true), m_iCurTheme(0), m_iCurLevel(0)
{

}


CDataManager::~CDataManager()
{
	CC_SAFE_DELETE(_dataManager);
}

CDataManager* CDataManager::getInstance()
{
	if (_dataManager == nullptr)
	{
		_dataManager = new CDataManager();
		_dataManager->Init();
	}

	return _dataManager;
}


//����json������
bool CDataManager::LoadMapData()
{
	string strContent = FileUtils::getInstance()->getStringFromFile(MAP_FILE_PATH);

	rapidjson::Document oDoc;
	oDoc.Parse<0>(strContent.c_str());
	if (oDoc.HasParseError())
	{
		log("Parse Error: %s\n", oDoc.GetParseError());
		return false;
	}
	if (oDoc.IsNull() || !oDoc.IsArray())
	{
		return false;
	}

	for (int i = 0; i < oDoc.Size(); i++)
	{
		MapInfo mapInfo;

		//color
		if (oDoc[i]["color"].IsNull())
		{
			return false;
		}
		mapInfo.iColor = oDoc[i]["color"].GetInt();

		//id
		if (oDoc[i]["id"].IsNull())
		{
			return false;
		}
		mapInfo.iID = oDoc[i]["id"].GetInt();

		//beat
		if (oDoc[i]["beat"].IsNull())
		{
			return false;
		}
		mapInfo.iBeat = oDoc[i]["beat"].GetInt();

		//map
		if (oDoc[i]["map"].IsNull() || (!oDoc[i]["map"].IsArray()))
		{
			return false;
		}

		//ÿ������
		for (int j = 0; j < oDoc[i]["map"].Size(); j++)
		{
			if (oDoc[i]["map"][j].IsNull() || (!oDoc[i]["map"][j].IsArray()))
			{
				return false;
			}
			for (int k = 0; k < oDoc[i]["map"][j].Size(); k++)
			{
				if (oDoc[i]["map"][j][k].IsNull())
				{
					return false;
				}

				int iValue = oDoc[i]["map"][j][k].GetInt();
				//ÿ�����ݴ���vecMap��
				mapInfo.vecMap.push_back(iValue);
			}
		}

		//mapitem
		if (oDoc[i]["mapitem"].IsNull() || (!oDoc[i]["mapitem"].IsArray()))
		{
			return false;
		}
		for (int j = 0; j < oDoc[i]["mapitem"].Size(); j++)
		{
			if (oDoc[i]["mapitem"][j].IsNull())
			{
				return false;
			}
			string strMapItem = oDoc[i]["mapitem"][j].GetString();

			int iArrow[3] = {0, 0, 0};
			SplitString(strMapItem, '_', iArrow);

			ArrowInfo arrowInfo;
			arrowInfo.iY = iArrow[0];
			arrowInfo.iX = iArrow[1];
			arrowInfo.iType = iArrow[2];

			//����vecMapItem��
			mapInfo.vecMapItem.push_back(arrowInfo);
		}
		
		//useritem
		if (oDoc[i]["useritem"].IsNull() || (!oDoc[i]["useritem"].IsArray()))
		{
			return false;
		}
		for (int j = 0; j < oDoc[i]["useritem"].Size(); j++)
		{
			if (oDoc[i]["useritem"][j].IsNull())
			{
				return false;
			}
			int iNum = oDoc[i]["useritem"][j].GetInt();

			mapInfo.vecUserItem.push_back(iNum);
		}

		m_oMapVec.push_back(mapInfo);
	}

	return true;
}


//�Ƴ�string������λ�ó��ֹ���ĳ���ַ�
void CDataManager::SplitString(string &srcStr, char chTarget, int* pArray)
{
	int iPos = srcStr.find_first_of('_');
	string strTemp = srcStr.substr(0, iPos);
	pArray[0] = atoi(strTemp.c_str());

	int iNewPos = srcStr.find_first_of('_', iPos + 1);
	strTemp = srcStr.substr(iPos + 1, iNewPos - iPos - 1);
	pArray[1] = atoi(strTemp.c_str());

	iPos = srcStr.find_first_of('_', iNewPos + 1);
	strTemp = srcStr.substr(iNewPos + 1, iPos - iNewPos - 1);
	pArray[2] = atoi(strTemp.c_str());
}


void CDataManager::InitUserData(const char* pName)
{
	rapidjson::Document oWriteDoc;
	oWriteDoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = oWriteDoc.GetAllocator();
	rapidjson::Value oArray(rapidjson::kArrayType);

	oWriteDoc.AddMember("score", 0, allocator);
	oWriteDoc.AddMember("level", 0, allocator);

	//��ʼ������
	m_stUserInfo.iLevel = 0;
	m_stUserInfo.iScore = 0;

	for (int i = 0; i < LEVELNUM; i++)
	{
		m_stUserInfo.vecData.push_back(0);
		oArray.PushBack(0, allocator);
	}

	oWriteDoc.AddMember("data", oArray, allocator);

	rapidjson::StringBuffer oBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> oWriter(oBuffer);
	oWriteDoc.Accept(oWriter);

	//д��
	FILE* pFile = fopen(pName, "wb+");
	if (pFile == NULL)
	{
		log("Open File '%s' Failed.", pName);
		return;
	}

	fputs(oBuffer.GetString(), pFile);
	fclose(pFile);
}


//�����û�����
bool CDataManager::LoadUserData()
{
	string strPath = FileUtils::getInstance()->getWritablePath() + string(SAVE_DATA_PATH);

#ifdef _DEBUG_
	log("UserData Path:%s\n", strPath.c_str());
#endif

	string strContent = FileUtils::getInstance()->getStringFromFile(strPath);
	rapidjson::Document oDoc;
	oDoc.Parse<0>(strContent.c_str());
	if (oDoc.HasParseError() || oDoc.IsNull())
	{
		//log("CDataManager::LoadUserData Load 'oDoc' Error.");
		InitUserData(strPath.c_str());
		return true;
	}

	//��ǰ��õķ���
	if (oDoc["score"].IsNull())
	{
		log("CDataManager::LoadUserData Load 'score' Error.");
		return false;
	}
	m_stUserInfo.iScore = oDoc["score"].GetInt();

	//��ͨ���ؿ���������Ϊ�����ܿ�ؿ�������Ҳ��ʾ��ǰͨ�������һ��
	if (oDoc["level"].IsNull())
	{
		log("CDataManager::LoadUserData Load 'level' Error.");
		return false;
	}
	m_stUserInfo.iLevel = oDoc["level"].GetInt();

	//���йؿ���ǰ��Ϣ
	if (oDoc["data"].IsNull() || (!oDoc["data"].IsArray()))
	{
		log("CDataManager::LoadUserData Load 'data' Error.");
		return false;
	}
	for (int i = 0; i < oDoc["data"].Size(); i++)
	{
		if (oDoc["data"][i].IsNull())
		{
			log("CDataManager::LoadUserData Load 'oDoc[data][%d]' Error.", i);
			return false;
		}

		int iValue = oDoc["data"][i].GetInt();
		m_stUserInfo.vecData.push_back(iValue);
	}

	return true;
}


//�û�����д�뵽ָ���ļ���
bool CDataManager::SaveUserData()
{
	string strPath = FileUtils::getInstance()->getWritablePath() + string(SAVE_DATA_PATH);
	string strContent = FileUtils::getInstance()->getStringFromFile(strPath);
	rapidjson::Document oDoc;
	oDoc.Parse<0>(strContent.c_str());
	if (oDoc.HasParseError())
	{
		log("Parse Error: %s\n", oDoc.GetParseError());
		return false;
	}
	if (oDoc.IsNull())
	{
		return false;
	}

	//���·���
	if (oDoc["score"].IsNull())
	{
		return false;
	}
	oDoc["score"].SetInt(m_stUserInfo.iScore);

	//����ͨ���Ĺؿ�����
	if (oDoc["level"].IsNull())
	{
		return false;
	}
	oDoc["level"].SetInt(m_stUserInfo.iLevel);

	//���¹ؿ���ǰ��Ϣ
	if (oDoc["data"].IsNull() || !oDoc["data"].IsArray())
	{
		return false;
	}
	int iVecSize = m_stUserInfo.vecData.size();
	int iJsonSize = oDoc["data"].Size();
	if (iVecSize != iJsonSize)
	{
		log("Error: %d(iVecSize) != %d(iJsonSize)\n", iVecSize, iJsonSize);
		return false;
	}
	for (rapidjson::SizeType i = 0; i < oDoc["data"].Capacity(); i++)
	{
		if (oDoc["data"][i].IsNull())
		{
			return false;
		}
		oDoc["data"][i].SetInt(m_stUserInfo.vecData[i]);
	}

	//׼��д������
	rapidjson::StringBuffer oBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> oWriter(oBuffer);
	oDoc.Accept(oWriter);

	FILE* pFile = fopen(strPath.c_str(), "wb+");
	if (pFile == NULL)
	{
		log("Error: Open File '%s' Failed.\n", strPath.c_str());
		return false;
	}

	fputs(oBuffer.GetString(), pFile);
	fclose(pFile);

#ifdef _DEBUG_
	log("UserDataPath:%s", strPath.c_str());
#endif

	return true;
}


//��õ�ͼ����
const MapInfo* CDataManager::GetMapInfo()
{
	int iActualLevel = GET_ACTUAL_LEVEL(m_iCurTheme, m_iCurLevel);
	return &m_oMapVec[iActualLevel];
}

const int INVALID_POS = -1;

//����û�����
int CDataManager::GetTotalScore()
{
	return m_stUserInfo.iScore;
}


//��ùؿ��ķ���
int CDataManager::GetLevelScore(int iTheme, int iLevel)
{
	int iActualLevel = GET_ACTUAL_LEVEL(iTheme, iLevel);
	return m_stUserInfo.vecData[iActualLevel];
}


//ͨ����ǰ�ؿ��ɹ�
void CDataManager::Pass(int iScore)
{
	//��ȡ�ؿ�
	int iActualLevel = GET_ACTUAL_LEVEL(m_iCurTheme, m_iCurLevel);

	//��ȡ�ɵķ���
	int iOldScore = m_stUserInfo.vecData[iActualLevel];

	//����Ƿ���Ҫ���·�����ֻ�ڵ�ǰ�����Ⱦɵķ�����ʱ����
	if (iOldScore < iScore)
	{
		//���µ����ؿ��������ܷ���
		m_stUserInfo.vecData[iActualLevel] = iScore;
		m_stUserInfo.iScore += iScore - iOldScore;
	}

	//����Ƿ���Ҫ���µ�ǰͨ���Ĺؿ���
	if (iOldScore <= 0)
	{
		int iTotalLevel = m_stUserInfo.vecData.size();
		if (m_stUserInfo.iLevel < iTotalLevel - 1)
		{
			m_stUserInfo.iLevel += 1;
		}
	}
}


//��ʼ��
bool CDataManager::Init()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_vecOrigin = Director::getInstance()->getVisibleOrigin();

	if (!this->LoadMapData())
	{
		return false;
	}

	if (!this->LoadUserData())
	{
		return false;
	}

	return true;
}


//���õ�ǰ����͹ؿ�
void CDataManager::SetCurThemeAndLevel(int iCurTheme, int iCurLevel)
{
#ifdef _DEBUG_
	log("SetCurThemeAndLevel: (%d, %d)->(%d, %d)", m_iCurTheme, m_iCurLevel, iCurTheme, iCurLevel);
#endif

	m_iCurLevel = iCurLevel;
	m_iCurTheme = iCurTheme;
}


//��õ�ǰ������ɫ
int CDataManager::GetCurThemeColor()
{
	return GetMapInfo()->iColor;
}


//��ȡδͨ���Ĺؿ�
int CDataManager::GetUnpassLevel()
{
	return m_stUserInfo.iLevel;
}


int CDataManager::GetThemeScore(int iTheme)
{
	int iScoreSum = 0;
	for (int i = iTheme * 20; i < (iTheme + 1) * 20; i++)
	{
		iScoreSum += m_stUserInfo.vecData[i];
	}

	return iScoreSum;
}