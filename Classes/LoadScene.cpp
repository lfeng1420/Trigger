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

	//0.5秒之后开始加载数据
	scheduleOnce(schedule_selector(CLoadScene::OnSwitchScene), 0.5f);

	return true;
}


//定时器调用
void CLoadScene::OnSwitchScene(float dt)
{
	//加载数据
	CDataManager::getInstance();
	CResManager::getInstance();

	//切换场景
	auto scene = CMenuScene::CreateScene();
	Director::getInstance()->replaceScene(scene);
}
