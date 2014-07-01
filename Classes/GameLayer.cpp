#include "GameLayer.h"
#include "AppDelegate.h"

GameLayer::GameLayer(){}

GameLayer::~GameLayer(){}

bool GameLayer::init(){
	if(Layer::init()) {
		//get the origin point of the X-Y axis, and the visiable size of the screen
		Size visiableSize = Director::getInstance()->getVisibleSize();
		Point origin = Director::getInstance()->getVisibleOrigin();

		this->gameStatus = GAME_STATUS_READY;
		this->score = 0;

        flapVelocity = FLAP_VELOCITY_WINK;
        gravity = GRAVITY_WINK;
        pipDistance = PIP_DISTANCE_WINK;
        pipInterval = PIP_INTERVAL_WINK;
        rotateRate = ROTATE_RATE_WINK;

		// Add the bird
		this->bird = BirdSprite::getInstance();
		this->bird->createBird();
		PhysicsBody *body = PhysicsBody::create();
        body->addShape(PhysicsShapeCircle::create(BIRD_RADIUS));
        body->setDynamic(true);
		body->setLinearDamping(0.0f);
		body->setGravityEnable(false);

		body->setCategoryBitmask(0x1);
		body->setContactTestBitmask(0x2);
		body->setCollisionBitmask(-1);

		this->bird->setPhysicsBody(body);
		this->bird->setPosition(origin.x + visiableSize.width*1/3 - 5,origin.y + visiableSize.height/2 + 5);
		this->bird->idle();
		this->addChild(this->bird);

        // Add the ground
        this->groundNode = Node::create();
        float landHeight = BackgroundLayer::getLandHeight();
        auto groundBody = PhysicsBody::create();
        groundBody->addShape(PhysicsShapeBox::create(Size(640, landHeight)));
        groundBody->setDynamic(false);

		groundBody->setCategoryBitmask(0x2);    // 0001
		groundBody->setContactTestBitmask(0x1); // 0100
		groundBody->setCollisionBitmask(-1);   // 0011

        groundBody->setLinearDamping(0.0f);
        this->groundNode->setPhysicsBody(groundBody);
        this->groundNode->setPosition(144, landHeight/2);
        this->addChild(this->groundNode);

        // init land
        this->landSpite1 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
        this->landSpite1->setAnchorPoint(Point::ZERO);
        this->landSpite1->setPosition(Point::ZERO);
        this->addChild(this->landSpite1, 30);

        this->landSpite2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
        this->landSpite2->setAnchorPoint(Point::ZERO);
        this->landSpite2->setPosition(this->landSpite1->getContentSize().width-2.0f,0);
        this->addChild(this->landSpite2, 30);

        this->landSpite3 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
        this->landSpite3->setAnchorPoint(Point::ZERO);
        this->landSpite3->setPosition(this->landSpite1->getContentSize().width * 2 -2.0f,0);
        this->addChild(this->landSpite3, 30);

		shiftLand = schedule_selector(GameLayer::scrollLand);
        this->schedule(shiftLand, 0.01f);

        this->scheduleUpdate();

		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

		return true;
	}else {
		return false;
	}
}

bool GameLayer::onContactBegin(PhysicsContact& contact) {
	CCLOG("contact begin");
	this->gameOver();
	return true;
}

void GameLayer::scrollLand(float dt){
	this->landSpite1->setPositionX(this->landSpite1->getPositionX() - 2.0f);
	this->landSpite2->setPositionX(this->landSpite1->getPositionX() + this->landSpite1->getContentSize().width - 2.0f);
	this->landSpite3->setPositionX(this->landSpite2->getPositionX() + this->landSpite1->getContentSize().width - 2.0f);

	if(this->landSpite2->getPositionX() == 0) {
		this->landSpite1->setPositionX(0);
	}

    if(this->landSpite3->getPositionX() == 0) {
        this->landSpite2->setPositionX(0);
    }

    // move the pips
    for (auto singlePip : this->pips) {
        singlePip->setPositionX(singlePip->getPositionX() - 2);
        if(singlePip->getPositionX() < -PIP_WIDTH) {
            singlePip->setTag(PIP_NEW);
            Size visibleSize = Director::getInstance()->getVisibleSize();
            singlePip->setPositionX(visibleSize.width);
            singlePip->setPositionY(this->getRandomHeight());
        }
    }
}

void GameLayer::onWink() {
    if (!tapMode) flapTheBird();
}

void GameLayer::onTouch() {
    if (tapMode) flapTheBird();
}

void GameLayer::flapTheBird() {

	if(this->gameStatus == GAME_STATUS_OVER) {
		return;
	}

	SimpleAudioEngine::getInstance()->playEffect("sfx_wing.ogg");
	if(this->gameStatus == GAME_STATUS_READY) {
		this->delegator->onGameStart();
		this->bird->fly();
		this->gameStatus = GAME_STATUS_START;
        this->createPips();
	}else if(this->gameStatus == GAME_STATUS_START) {
		this->bird->getPhysicsBody()->setVelocity(Vect(0, flapVelocity));
	}
}

void GameLayer::rotateBird() {
    float verticalSpeed = this->bird->getPhysicsBody()->getVelocity().y;
    this->bird->setRotation(max(min(65, (verticalSpeed*rotateRate - 60)), -40));
}


void GameLayer::update(float delta) {
    if (this->gameStatus == GAME_STATUS_START) {
        this->rotateBird();
		this->checkHit();
    }
}

void GameLayer::createPips() {
    // Create the pips
    for (int i = 0; i < PIP_COUNT; i++) {
        Size visibleSize = Director::getInstance()->getVisibleSize();

        Sprite *pipUp = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_up"));
        Sprite *pipDown = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_down"));
        Node *singlePip = Node::create();

        // bind to pair
        pipDown->setPosition(0, PIP_HEIGHT + pipDistance);
		singlePip->addChild(pipDown, 0, DOWN_PIP);
        singlePip->addChild(pipUp, 0, UP_PIP);
        singlePip->setPosition(visibleSize.width + i*pipInterval + WAIT_DISTANCE, this->getRandomHeight());
		auto body = PhysicsBody::create();
		auto shapeBoxDown = PhysicsShapeBox::create(pipDown->getContentSize(),PHYSICSSHAPE_MATERIAL_DEFAULT, Point(0, PIP_HEIGHT + pipDistance));
		body->addShape(shapeBoxDown);
		body->addShape(PhysicsShapeBox::create(pipUp->getContentSize()));
		body->setDynamic(false);

		body->setCategoryBitmask(0x2);
		body->setContactTestBitmask(0x1);
		body->setCollisionBitmask(-1);

		singlePip->setPhysicsBody(body);
        singlePip->setTag(PIP_NEW);
        
        this->addChild(singlePip);
        this->pips.push_back(singlePip);
    }
}

int GameLayer::getRandomHeight() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return (rand()%(int)(PIPE_RAND_RANGE)) - (PIPE_RAND_MIN * -1);
}


void GameLayer::checkHit() {
    for(auto pip : this->pips) {
		if (pip->getTag() == PIP_NEW) {
            if (pip->getPositionX() < this->bird->getPositionX()) {
				SimpleAudioEngine::getInstance()->playEffect("sfx_point.ogg");
                this->score ++;
                this->delegator->onGamePlaying(this->score);
                pip->setTag(PIP_PASS);
            }
        }
    }
}

void GameLayer::gameOver() {
    if(this->gameStatus == GAME_STATUS_OVER) {
        return;
    }
	SimpleAudioEngine::getInstance()->playEffect("sfx_hit.ogg");
	//get the best score
	int bestScore = UserRecord::getInstance()->readIntegerFromUserDefault("best_score");
	//update the best score
	if(this->score > bestScore){
		UserRecord::getInstance()->saveIntegerToUserDefault("best_score",this->score);
	}
	this->delegator->onGameEnd(this->score, bestScore);
	this->unschedule(shiftLand);
	SimpleAudioEngine::getInstance()->playEffect("sfx_die.ogg");
	this->bird->die();
	this->bird->setRotation(90);
	this->birdSpriteFadeOut();
	this->gameStatus = GAME_STATUS_OVER;
}

void GameLayer::setTapMode(){

    tapMode = true;
    flapVelocity = FLAP_VELOCITY_TAP;
    gravity = GRAVITY_TAP;
    pipDistance = PIP_DISTANCE_TAP;
    pipInterval = PIP_INTERVAL_TAP;
    rotateRate = ROTATE_RATE_TAP;

}


void GameLayer::birdSpriteFadeOut(){
    //changes from fadeout - fixed issue with bird vanishing
	FadeIn* animation = FadeIn::create(1.5);
	CallFunc* animationDone = CallFunc::create(bind(&GameLayer::birdSpriteRemove,this));
	Sequence* sequence = Sequence::createWithTwoActions(animation,animationDone);
	this->bird->stopAllActions();
	this->bird->runAction(sequence);
}

void GameLayer::birdSpriteRemove(){
	this->bird->setRotation(0);
	this->removeChild(this->bird);
}
