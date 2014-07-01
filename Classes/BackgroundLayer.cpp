#include "BackgroundLayer.h"
BackgroundLayer::BackgroundLayer(){};
BackgroundLayer::~BackgroundLayer(){};

bool BackgroundLayer::init(){
	if(!Layer::init()){
		return false;
	}

	Sprite *background;
    background = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("bg_day"));

	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	this->addChild(background);

	Sprite *background2;
    background2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("bg_day"));

	background2->setAnchorPoint(Point::ZERO);
	background2->setPosition(Point(background2->getContentSize().width - 1 ,0));
	this->addChild(background2);

	Sprite *background3;
    background3 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("bg_day"));

	background3->setAnchorPoint(Point::ZERO);
	background3->setPosition(Point(background3->getContentSize().width * 2 - 2 ,0));
	this->addChild(background3);


	return true;
}

float BackgroundLayer::getLandHeight() {
    return Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"))->getContentSize().height;
}