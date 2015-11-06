#include "ResManager.h"
#include "GlobalDef.h"

USING_NS_CC;

static CResManager* _resManager = nullptr;

CResManager::CResManager()
{
}


CResManager::~CResManager()
{
	CC_SAFE_DELETE(_resManager);
}

CResManager* CResManager::getInstance()
{
	if (_resManager == nullptr)
	{
		_resManager = new CResManager();
		_resManager->Init();
	}

	return _resManager;
}

//≥ı ºªØ
bool CResManager::Init()
{
	// ≥ı ºªØ
	m_audioCache = CocosDenshion::SimpleAudioEngine::getInstance();
	m_animCache = AnimationCache::getInstance();
	m_frameCache = SpriteFrameCache::getInstance();

	char* arrPlistName[] = {
		"plists/block.plist",
		"plists/buttonRes.plist",
		"plists/effer.plist",
		"plists/item.plist",
		"plists/lock.plist",
		"plists/missionstar.plist",
		"plists/num_0.plist",
		"plists/num_1.plist",
		"plists/num_2.plist",
		"plists/textRes.plist",
		"plists/theme.plist"
	};
	for (int i = 0; i < 11; i++)
	{
		LoadPlist(arrPlistName[i]);
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	char* arrSoundName[7] = {
		"sounds/sound_change.wav",
		"sounds/sound_complete.wav",
		"sounds/sound_dead.wav",
		"sounds/sound_item.wav",
		"sounds/sound_star1.wav",
		"sounds/sound_star2.wav",
		"sounds/sound_star3.wav"
	};
	for (int i = 0; i < 7; i++)
	{
		PreloadSound(arrSoundName[i]);
	}
#else
	char* arrSoundName[7] = {
		"sounds/sound_change.mp3",
		"sounds/sound_complete.mp3",
		"sounds/sound_dead.mp3",
		"sounds/sound_item.mp3",
		"sounds/sound_star1.mp3",
		"sounds/sound_star2.mp3",
		"sounds/sound_star3.mp3"
	};
	for (int i = 0; i < 7; i++)
	{
		PreloadSound(arrSoundName[i]);
	}
#endif

	//…˘“Ù◊¥Ã¨
	m_bSoundState = UserDefault::getInstance()->getBoolForKey("SoundState", true);

	return true;
}


/*
void CResManager::SearchFile(const char* pDir, const char* pFileExtension)
{
	_finddata_t stFindData;
	string strPath(pDir);
	strPath.append("//");
	string strFindPath = strPath;
	strFindPath.append(pFileExtension);
	long lHandle = _findfirst(strFindPath.c_str(), &stFindData);
	if (-1L == lHandle)
	{
		return;
	}

	do
	{
		string strRoute = strPath;
		strRoute.append(stFindData.name);
		if (strRoute.find(".plist") != string::npos)
		{
			LoadPlist(strRoute.c_str());
		}
		else if (strRoute.find(".mp3") != string::npos)
		{
			PreloadSound(strRoute.c_str());
		}

	} while (_findnext(lHandle, &stFindData) == 0);
	_findclose(lHandle);
}
*/

void CResManager::LoadPlist(const char* pName)
{
#ifdef _DEBUG_
	log("LoadPlist:%s", pName);
#endif
	m_frameCache->addSpriteFramesWithFile(pName);
}


void CResManager::RemovePlist(const char* pName)
{
	m_frameCache->removeSpriteFramesFromFile(pName);
}


SpriteFrame* CResManager::GetSpriteFrame(char* pName)
{
	SpriteFrame* spriteFrame = m_frameCache->getSpriteFrameByName(pName);
	if (spriteFrame == nullptr)
	{
		log("Get SpriteFrame '%s' Error.", pName);
	}

	return spriteFrame;
}

void CResManager::AddAnim(char* pName[], int iLen, float fDelay, char* pAnimName)
{
#ifdef _DEBUG_
	log("------CResManager::addAnimation Load %s------", pAnimName);
	log("len: %d, delay: %f\nname:", iLen, fDelay);
#endif

	auto anim = Animation::create();
	anim->setDelayPerUnit(fDelay);

	for (int i = 0; i < iLen; i++)
	{
		anim->addSpriteFrameWithFileName(pName[i]);
#ifdef _DEBUG_
		log("%s", pName[i]);
#endif
	}

	m_animCache->addAnimation(anim, pAnimName);
}

Animate* CResManager::GetAnim(char* pName)
{
	auto anim = m_animCache->getAnimation(pName);
	if (anim == nullptr)
	{
		log("Get Animation '%s' Error.", pName);
		return nullptr;
	}

	return Animate::create(anim);
}


void CResManager::PreloadSound(const char* pName)
{
#ifdef _DEBUG_
	log("PreloadSound:%s", pName);
#endif
	m_audioCache->preloadEffect(pName);
}


//≤•∑≈“Ù–ß
void CResManager::PlayEffect(const char* pName)
{
	if (m_bSoundState)
	{
		string strFileName(pName);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
		strFileName.append(".wav");
#else
		strFileName.append(".mp3");
#endif

#ifdef _DEBUG_
		log("PlayEffect: %s", strFileName.c_str());
#endif
		m_audioCache->playEffect(strFileName.c_str());
	}
}


//…Ë÷√…˘“Ù◊¥Ã¨Œ™÷∏∂®◊¥Ã¨
void CResManager::SetSoundState(bool bSoundState)
{
	m_bSoundState = bSoundState;
}

//ªÒ»°…˘“Ù◊¥Ã¨
bool CResManager::GetSoundState()
{
	return m_bSoundState;
}


//±£¥Ê…˘“Ù◊¥Ã¨
void CResManager::SaveSoundState()
{
	UserDefault::getInstance()->setBoolForKey("SoundState", m_bSoundState);
}