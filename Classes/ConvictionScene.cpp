#include "ConvictionScene.h"

USING_NS_CC;

Scene *ConvictionScene::createScene() {

    return ConvictionScene::create();

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool ConvictionScene::init() {
    //////////////////////////////
    // 1. super init first
    if (!Scene::initWithPhysics()) {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Device::setAccelerometerEnabled(true);
    auto accListener = EventListenerAcceleration::create(CC_CALLBACK_2(ConvictionScene::OnAcceleration, this));
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(accListener, this);
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(ConvictionScene::onKeyReleased, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    auto backgroundSprite = Sprite::create("bg2.jpg");
    backgroundSprite->setScale(visibleSize.width / backgroundSprite->getContentSize().width,
                               visibleSize.height / backgroundSprite->getContentSize().height);
    backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(backgroundSprite);

    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    edgeNode->setPhysicsBody(edgeBody);
    edgeBody->setCategoryBitmask(0x03);
    edgeBody->setCollisionBitmask(0x01);
    addChild(edgeNode);
//    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    snowParticle = ParticleSnow::create();
    rainParticle = ParticleRain::create();
    snowParticle->setGravity(Vec2(5, -20));
    snowParticle->setTotalParticles(500);
    addChild(snowParticle);
    addChild(rainParticle);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(ConvictionScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    convictionLabel = Label::createWithTTF("1", "fonts/Marker Felt.ttf", 40);
    convictionLabel->setColor(Color3B(255, 215, 0));
    convictionLabel->setPosition(Point(visibleSize.width * 0.97, visibleSize.height * 0.97));
    addChild(convictionLabel, 1000);

    UserDefault *def = UserDefault::getInstance();
    auto convictions = def->getIntegerForKey("CONVICTIONS", 1);
    this->convictionCount = convictions;
    convictionLabel->setString(std::to_string(convictions));
    int width = visibleSize.width;
    int height = visibleSize.height;
    while (convictions--)
        this->addRandomElement(Vec2(rand() % width, rand() % height));

//    auto particle = ParticleSystemQuad::create("res/p4.plist");
//    particle->setPosition(200,200);
//    addChild(particle);
    return true;
}

void ConvictionScene::menuCloseCallback(Ref *pSender) {
    UserDefault *def = UserDefault::getInstance();
    def->flush();
    Director::getInstance()->end();
}

void ConvictionScene::OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event) {
    this->getPhysicsWorld()->setGravity(Vec2(acc->x * 600, acc->y * 600));
}

bool ConvictionScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    ++convictionCount;
    auto location = touch->getStartLocation();
    addRandomElement(location);
    UserDefault *def = UserDefault::getInstance();
    def->setIntegerForKey("CONVICTIONS", convictionCount);
//    def->flush();
    convictionLabel->setString(std::to_string(convictionCount));
    return true;
}

void ConvictionScene::addRandomElement(cocos2d::Vec2 position) {
    auto type =rand() % 14;
    Node *particle;
    switch (type) {
        case 0:
            particle = ParticleGalaxy::create();
            particle->setScale(0.7);
            break;
        case 1:
            particle = ParticleMeteor::create();
            particle->setScale(0.7);
            break;
        case 2:
            particle = ParticleFlower::create();
            particle->setScale(0.7);
            break;
        case 3:
            particle = ParticleFire::create();
            particle->setScale(0.4);
            break;
        case 4:
            particle = ParticleSun::create();
            particle->setScale(0.7);
            break;
        case 5:
            particle = ParticleSystemQuad::create("res/p1.plist");
            particle->setScale(0.2);
            break;
        case 6:
            particle = ParticleSystemQuad::create("res/p2.plist");
            particle->setScale(0.25);
            break;
        case 7:
            particle = ParticleSystemQuad::create("res/p3.plist");
            particle->setScale(0.2);
            break;
        case 8:
            particle = ParticleSystemQuad::create("res/p4.plist");
            particle->setScale(0.4);
            break;
        case 9:
            particle = ParticleSystemQuad::create("res/p5.plist");
            particle->setScale(0.4);
            break;
        case 10:
            particle = ParticleSystemQuad::create("res/p6.plist");
            particle->setScale(0.5);
            break;
            break;
        case 11:
            particle = ParticleSystemQuad::create("res/p7.plist");
            particle->setScale(0.2);
            break;
        case 12:
            particle = ParticleSystemQuad::create("res/p8.plist");
            particle->setScale(0.2);
            break;
        case 13:
            particle = ParticleSystemQuad::create("res/p9.plist");
            particle->setScale(0.15);
            break;
        default:
            break;
    }
    auto spriteBody = PhysicsBody::createCircle(20, PhysicsMaterial(0.2, 0.5, 0));
    spriteBody->setCollisionBitmask(1);
    spriteBody->setContactTestBitmask(true);
    particle->setPhysicsBody(spriteBody);
    particle->setPosition(position);
    addChild(particle);
}

void ConvictionScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
        menuCloseCallback(this);
    }
}