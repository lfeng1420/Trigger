#include "LoadScene.h"
#include "DataManager.h"
#include "ResManager.h"
#include "MenuScene.h"

USING_NS_CC;

CLoadScene::CLoadScene()
{
}


CLoadScene::~CLoadScene()
{
}


//cocos function
Scene* CLoadScene::CreateScene()
{
	auto layer = CLoadScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
}

//cocos function
bool CLoadScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bgSprite = Sprite::create("images/background.png");
	bgSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + origin);
	addChild(bgSprite);

	auto loadSprite = Sprite::create("images/loading.png");
	loadSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + origin);
	addChild(loadSprite);

	//0.5��֮��ʼ��������
	scheduleOnce(schedule_selector(CLoadScene::OnSwitchScene), 0.5f);

	return true;
}


//��ʱ������
void CLoadScene::OnSwitchScene(float dt)
{
	//��������
	CDataManager::getInstance();
	CResManager::getInstance();

	//�л�����
	auto scene = CMenuScene::CreateScene();
	Director::getInstance()->replaceScene(scene);
}
