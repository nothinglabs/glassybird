//
//  OptionLayer.cpp
//  parasite-city
//
//  Created by Zhou.Zeyong on 14-2-18.
//
//

#include "OptionLayer.h"



OptionLayer::OptionLayer()
{}

OptionLayer::~OptionLayer()
{}

bool OptionLayer::init()
{
	if(Layer::init()){
		auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesBegan = CC_CALLBACK_2(OptionLayer::onTouchesBegan, this);
        listener->onTouchesEnded = CC_CALLBACK_2(OptionLayer::onTouchesEnded, this);
        dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		return true;
	}else {
		return false;
	}
}

void OptionLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *event)
{
	this->delegator->onTouch();
	Point position = touches[0]->getLocation();
	lastTouchY = position.y;
}

void OptionLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
	Point position = touches[0]->getLocation();
	if (lastTouchY - position.y > 60) CCDirector::sharedDirector()->end();
}
