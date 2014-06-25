#include "WelcomeScene.h"
#include "OptionLayer.h"
#include "WelcomeLayer.h"

WelcomeScene::WelcomeScene() {
}
;

WelcomeScene::~WelcomeScene() {
}
;

bool WelcomeScene::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!Scene::init());
		auto _welcomeLayer = WelcomeLayer::create();
		//auto _welcomeLayer = StatusLayer::create();
		CC_BREAK_IF(!_welcomeLayer);
		this->addChild(_welcomeLayer);
		bRet = true;

		// Add operation layer to control the game
		auto optionLayer = OptionLayer::create();
		if (optionLayer) {
			optionLayer->setDelegator(_welcomeLayer);
			this->addChild(optionLayer);
		}

	} while (0);
	return bRet;
}

