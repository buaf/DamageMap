#include "standarttank.h"

StandartTank::StandartTank(b2World * world2d, float x_poss, float y_poss, int ownerId)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x_poss, y_poss);
    bodyDef.linearDamping = 1.0f;
    bodyDef.angularDamping = 1.0f;

    tankBody = world2d->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(11.0f, 11.0f); //квадратный

    b2FixtureDef fixtureDef;
    fixtureDef.restitution = 0.0f;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 10.0f; //плотность
    fixtureDef.friction = 0.3f; //коефициент трения

    tankBody->CreateFixture(&fixtureDef);
}

StandartTank::~StandartTank()
{
    destroyUnit();
    delete bulletBase;
}

void StandartTank::moveForward()
{
    float ange = tankBody->GetAngle();
    b2Vec2 vec = tankBody->GetPosition();
    vec.Set(vec.x + cos(ange), vec.y + sin(ange));
    tankBody->SetTransform(vec, tankBody->GetAngle());
}

void StandartTank::moveBack()
{
    float ange = tankBody->GetAngle();
    b2Vec2 vec = tankBody->GetPosition();
    vec.Set(vec.x - cos(ange), vec.y - sin(ange));
    tankBody->SetTransform(vec, tankBody->GetAngle());
}

float StandartTank::getGunAnge()
{
    return gunAngle;
}

float StandartTank::getBodyAnge()
{
    tankBody->GetAngle();
}

float StandartTank::getXpos()
{
    tankBody->GetPosition().x;
}

float StandartTank::getYpos()
{
    tankBody->GetPosition().y;
}

void  StandartTank::setGunAnge(float angle)
{
    gunAngle = angle;
}

void  StandartTank::setBodyAnge(float angle)
{
    tankBody->SetTransform(tankBody->GetPosition(), angle);
}

void  StandartTank::setXpos(float x)
{
    b2Vec2 vec = tankBody->GetPosition();
    vec.x = x;
    tankBody->SetTransform(vec, tankBody->GetAngle());
}

void  StandartTank::setYpos(float y)
{
    b2Vec2 vec = tankBody->GetPosition();
    vec.y = y;
    tankBody->SetTransform(vec, tankBody->GetAngle());
}

int StandartTank::getHealth()
{
    return health;
}

void StandartTank::setHealth(int hp)
{
    health = hp;
}

int StandartTank::getId()
{
    return id;
}

int StandartTank::setId(int id)
{
    this->id = id;
}

int StandartTank::getScore()
{
    return score;
}

int StandartTank::setScore(int score)
{
    this->score = score;
}

int StandartTank::getType()
{
    return type;
}

int StandartTank::setType(int type)
{
    this->type = type;
}

void StandartTank::destroyUnit()
{
    world->DestroyBody(tankBody);
}

b2Body * StandartTank::getBody()
{
    return tankBody;
}

QSet<AbstractBullet *> * StandartTank::getBulletBase()
{
    return bulletBase;
}
