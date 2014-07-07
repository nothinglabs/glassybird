#include "StatusLayer.h"
#include <string>

StatusLayer::StatusLayer(){};

StatusLayer::~StatusLayer(){};


bool StatusLayer::init(){
	if(!Layer::init()){
		return false;
	}
	// init numbers
    this->bestScore = 0;
    this->currentScore = 0;
	this->isNewRecord = false;
	Number::getInstance()->loadNumber(NUMBER_FONT.c_str(), "font_0%02d", 48);
    Number::getInstance()->loadNumber(NUMBER_SCORE.c_str(), "number_score_%02d");
	this->visibleSize = Director::getInstance()->getVisibleSize();
	this->originPoint = Director::getInstance()->getVisibleOrigin();
	this->showReadyStatus();
	this->loadWhiteSprite();

	scoreLabel = CCLabelTTF::create("0", "Arial", 40,
                                              CCSizeMake(595, 55), kCCTextAlignmentRight);
    scoreLabel->setPosition(ccp(180,334));
    scoreLabel->setColor(ccc3(200,0,0));
    scoreLabel->enableStroke(ccWHITE, 3.0,true);
    this->addChild(scoreLabel);
    scoreSprite->setTag(CURRENT_SCORE_SPRITE_TAG);

    tapOrWinkLabel = CCLabelTTF::create("", "Helvetica", 27,
                                          CCSizeMake(385, 55), kCCTextAlignmentLeft);
    tapOrWinkLabel->setPosition(ccp(202,330));
    tapOrWinkLabel->enableStroke(ccWHITE, 1.5,true);
    this->addChild(tapOrWinkLabel);
    tapOrWinkLabel->setColor(ccc3(10,92,10));
    tapOrWinkLabel->setString("Double-blink to flap!");


    CCLabelTTF* startLabel3 = CCLabelTTF::create("Rich Olson / nothinglabs.com", "Helvetica", 16,
                                          CCSizeMake(588, 52), kCCTextAlignmentRight);
    startLabel3->setPosition(ccp(339,4));
    this->addChild(startLabel3);
    startLabel3->setColor(ccc3(0,0,0));


	return true;
}

void StatusLayer::showReadyStatus() {
	scoreSprite = (Sprite *)Number::getInstance()->convert(NUMBER_FONT.c_str(), 0);
	scoreSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2,this->originPoint.y + this->visibleSize.height *5/6));
	this->addChild(scoreSprite);

}

void StatusLayer::showStartStatus() {

}

void StatusLayer::displayTapMode(bool tapMode) {

    if (tapMode) tapOrWinkLabel->setString("Tap to flap!");

}



void StatusLayer::onGameStart(){
	this->showStartStatus();
}

void StatusLayer::onGamePlaying(int score){
	const char *scoreString = String::createWithFormat("%d", score)->getCString();
	scoreLabel-> setString(scoreString);

}

void StatusLayer::onGameEnd(int curScore, bool tapMode, int bestTapScore, int bestBlinkScore){

	this->isNewRecord = false;

    if(tapMode && curScore > bestTapScore){
		this->isNewRecord = true;
        bestTapScore = curScore;
	}

    if(!tapMode && curScore > bestBlinkScore){
        this->isNewRecord = true;
        bestBlinkScore = curScore;
    }

	this->removeChild(scoreSprite);
	this->blinkFullScreen();

    CCLabelTTF* startLabel = CCLabelTTF::create("Tap for 'Tap Mode'\nDouble-Blink for 'Blink Mode'", "Helvetica", 37,
                                          CCSizeMake(485, 202), kCCTextAlignmentCenter);
    startLabel->setPosition(ccp(315,35));
    this->addChild(startLabel);

    CCLabelTTF* startLabel2 = CCLabelTTF::create("Problems? Calibrate wink detection in Glass settings!", "Helvetica", 16,
                                          CCSizeMake(585, 41), kCCTextAlignmentRight);
    startLabel2->setPosition(ccp(89,10));
    this->addChild(startLabel2);
    startLabel2->setColor(ccc3(0,0,0));



    CCLabelTTF* highScoreLabel = CCLabelTTF::create("NEW HIGH SCORE!", "Helvetica", 30,
                                              CCSizeMake(285, 41), kCCTextAlignmentLeft);
    highScoreLabel->setPosition(ccp(331,240));
    if (this->isNewRecord) this->addChild(highScoreLabel);
    highScoreLabel->setColor(ccc3(200,0,0));


    char blinkScoreTitle[255];

    sprintf(blinkScoreTitle, "Best Blinking Score: %d", bestBlinkScore);

    CCLabelTTF* blinkLabel = CCLabelTTF::create(blinkScoreTitle, "Helvetica", 34,
                                              CCSizeMake(405, 48), kCCTextAlignmentLeft);
    blinkLabel->setPosition(ccp(335,213));
    this->addChild(blinkLabel);
    blinkLabel->setColor(ccc3(0,60,0));


    char tapScoreTitle[255];

    sprintf(tapScoreTitle, "Best Tapping Score: %d", bestTapScore);

    CCLabelTTF* tapScore = CCLabelTTF::create(tapScoreTitle, "Helvetica", 34,
                                              CCSizeMake(405, 48), kCCTextAlignmentLeft);
    tapScore->setPosition(ccp(335,170));
    this->addChild(tapScore);
    tapScore->setColor(ccc3(0,0,90));

}

void StatusLayer::loadWhiteSprite(){
	//this white sprite is used for blinking the screen for a short while
	whiteSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("white"));
	whiteSprite->setScale(100);
	whiteSprite->setOpacity(0);
	this->addChild(whiteSprite,10000);
}

void StatusLayer::blinkFullScreen(){
	//display a flash blink
	auto fadeOut = FadeOut::create(0.1f);
	auto fadeIn = FadeIn::create(0.1f);
	auto blinkAction = Sequence::create(fadeIn,fadeOut,NULL);
    CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::fadeInGameOver, this));
    auto sequence = Sequence::createWithTwoActions(blinkAction, actionDone);
    whiteSprite->stopAllActions();
	whiteSprite->runAction(sequence);
}

void StatusLayer::fadeInGameOver(){    
    // create the game over panel
	Sprite* gameoverSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("text_game_over"));
	gameoverSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y + this->visibleSize.height *3/4));
	this->addChild(gameoverSprite);
	auto gameoverFadeIn = FadeIn::create(0.5f);
    
    // Start next action
	CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::jumpToScorePanel, this));
    auto sequence = Sequence::createWithTwoActions(gameoverFadeIn, actionDone);
    gameoverSprite->stopAllActions();
	gameoverSprite->runAction(sequence);
}

void StatusLayer::jumpToScorePanel(){    
    // create the score panel
    Sprite* scorepanelSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("score_panel"));
	scorepanelSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y - scorepanelSprite->getContentSize().height));
	this->addChild(scorepanelSprite);
        
	//display the  best score on the score panel
	auto bestScoreSprite = (Sprite *)Number::getInstance()->convert(NUMBER_SCORE.c_str(), this->bestScore, Gravity::GRAVITY_RIGHT);
	bestScoreSprite->setAnchorPoint(Point(1, 1));
	bestScoreSprite->setPosition(scorepanelSprite->getContentSize().width - 28 ,
		50);
	scorepanelSprite->addChild(bestScoreSprite);
    
    
	string medalsName = this->getMedalsName(currentScore);
	if(medalsName != "") {
		Sprite* medalsSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName(medalsName));
		medalsSprite->addChild(this->blink);
		medalsSprite->setPosition(54, 58);
		scorepanelSprite->addChild(medalsSprite);
	}
    
	//if the current score is higher than the best score.
	//the panel will appear a "new" tag.
	if(this->isNewRecord){
		Sprite* newTagSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("new"));
		newTagSprite->setPosition(-16, 12);
		bestScoreSprite->addChild(newTagSprite);
	}
	
    // Start next action
	auto scorePanelMoveTo = MoveTo::create(0.8f ,Point(this->originPoint.x + this->visibleSize.width / 2,this->originPoint.y + this->visibleSize.height/2 - 10.0f));
	// add variable motion for the action
	EaseExponentialOut* sineIn = EaseExponentialOut::create(scorePanelMoveTo);
	CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::refreshScoreCallback, this));
    auto sequence = Sequence::createWithTwoActions(sineIn, actionDone);
    scorepanelSprite->stopAllActions();
	scorepanelSprite->runAction(sequence);
}

void StatusLayer::refreshScoreCallback(){
	this->tmpScore = 0;
	schedule(schedule_selector(StatusLayer::refreshScoreExecutor),0.1f);
	tapToRestart = true;

}

void StatusLayer::refreshScoreExecutor(float dt){
	if(this->getChildByTag(CURRENT_SCORE_SPRITE_TAG)){
		this->removeChildByTag(CURRENT_SCORE_SPRITE_TAG);
	}
	scoreSprite = (Sprite *)Number::getInstance()->convert(NUMBER_SCORE.c_str(), this->tmpScore, Gravity::GRAVITY_RIGHT);
	scoreSprite->setAnchorPoint(Point(1,0));
	scoreSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width * 3 / 4 + 20.0f, this->originPoint.y + this->visibleSize.height *  1 / 2));
	scoreSprite->setTag(CURRENT_SCORE_SPRITE_TAG);
	this->addChild(scoreSprite,1000);
	this->tmpScore++;
	if(this->tmpScore > this->currentScore){
		unschedule(schedule_selector(StatusLayer::refreshScoreExecutor));
	}

}

void StatusLayer::setBlinkSprite() {
}

string StatusLayer::getMedalsName(int score){
	this->setBlinkSprite();

	//display the golden silver or bronze iron
	string medalsName = "";
	if(this->currentScore >=10 && this->currentScore < 20){//iron medals
		medalsName = "medals_0";
	}else if(this->currentScore >= 20 && currentScore < 30){//bronze medals
		medalsName = "medals_1";
	}else if(currentScore >=30 && currentScore < 50){//silver medals
		medalsName = "medals_2";
	}else if(currentScore >=50){//golden medals
		medalsName = "medals_3";
	}
	return medalsName;
}

void StatusLayer::menuRestartCallback(Object* pSender){
	SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
    auto scene = GameScene::create();
    TransitionScene *transition = TransitionFade::create(1, scene);
    Director::getInstance()->replaceScene(transition);
}


void StatusLayer::onTouch() {
	if (tapToRestart) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
        this->removeChildByTag(BIRD_SPRITE_TAG);
        auto scene = GameScene::create();
        scene->tapMode = true;
        scene->prep();
        TransitionScene *transition = TransitionFade::create(1, scene);
        Director::getInstance()->replaceScene(transition);
    }

}

void StatusLayer::onWink() {
	if (tapToRestart) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
        this->removeChildByTag(BIRD_SPRITE_TAG);
        auto scene = GameScene::create();
        scene->tapMode = false;
        scene->prep();
        TransitionScene *transition = TransitionFade::create(1, scene);
        Director::getInstance()->replaceScene(transition);
    }
}


