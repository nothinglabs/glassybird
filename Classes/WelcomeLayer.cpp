#include "WelcomeLayer.h"
#include "AppDelegate.h"

WelcomeLayer::WelcomeLayer(){};

WelcomeLayer::~WelcomeLayer(){};

bool WelcomeLayer::init(){
	if(!Layer::init()){
		return false;
	}

	//get the origin point of the X-Y axis, and the visiable size of the screen
	Size visiableSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

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


	//add the word game-title to the current scene
	Sprite *title  = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("title"));
	title->setPosition(Point(origin.x + visiableSize.width/2 , (visiableSize.height * 5) / 7));
	this->addChild(title);


	//create a bird and set the position in the center of the screen
	this->bird = BirdSprite::getInstance();
	this->bird->createBird();
	this->bird->setTag(BIRD_SPRITE_TAG);
	this->bird->setPosition(Point(origin.x + visiableSize.width / 2,origin.y + visiableSize.height*3/5 - 10));
	this->bird->idle();
	this->addChild(this->bird);

	// Add the land
	this->land1 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
	this->land1->setAnchorPoint(Point::ZERO);
	this->land1->setPosition(Point::ZERO);
	this->addChild(this->land1);

	this->land2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
	this->land2->setAnchorPoint(Point::ZERO);
	this->land2->setPosition(this->land1->getContentSize().width - 2.0f, 0);
	this->addChild(this->land2);

	this->land3 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
    this->land3->setAnchorPoint(Point::ZERO);
    this->land3->setPosition(this->land1->getContentSize().width * 2 - 2.0f, 0);
    this->addChild(this->land3);

	this->schedule(schedule_selector(WelcomeLayer::scrollLand), 0.01f);

	CCLabelTTF* startLabel = CCLabelTTF::create("Tap for 'Tap Mode'\nDouble-Blink for 'Blink Mode'", "Helvetica", 37,
                                          CCSizeMake(485, 202), kCCTextAlignmentCenter);
    startLabel->setPosition(ccp(315,35));
	this->addChild(startLabel);

    CCLabelTTF* startLabel2 = CCLabelTTF::create("Be sure to calibrate wink detection in Glass settings!!!", "Helvetica", 23,
                                          CCSizeMake(595, 55), kCCTextAlignmentRight);
    startLabel2->setPosition(ccp(293,10));
    this->addChild(startLabel2);
    startLabel2->setColor(ccc3(0,0,0));

    CCLabelTTF* startLabel3 = CCLabelTTF::create("Rich Olson / nothinglabs.com", "Helvetica", 23,
                                          CCSizeMake(595, 55), kCCTextAlignmentRight);
    startLabel3->setPosition(ccp(22,330));
    this->addChild(startLabel3);
    startLabel3->setColor(ccc3(0,0,0));


	return true;
}

void WelcomeLayer::scrollLand(float dt){
	this->land1->setPositionX(this->land1->getPositionX() - 2.0f);
	this->land2->setPositionX(this->land1->getPositionX() + this->land1->getContentSize().width - 2.0f);
    this->land3->setPositionX(this->land2->getPositionX() + this->land1->getContentSize().width - 2.0f);

	if(this->land2->getPositionX() == 0) {
		this->land1->setPositionX(0);
	}

    if(this->land3->getPositionX() == 0) {
        this->land2->setPositionX(0);
    }

}

void WelcomeLayer::onTouch() {
	SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
	this->removeChildByTag(BIRD_SPRITE_TAG);
	auto scene = GameScene::create();
	scene->tapMode = true;
    scene->prep();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

void WelcomeLayer::onWink() {
	SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
	this->removeChildByTag(BIRD_SPRITE_TAG);
	auto scene = GameScene::create();
	scene->tapMode = false;
	scene->prep();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

