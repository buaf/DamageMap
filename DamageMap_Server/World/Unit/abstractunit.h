#ifndef ABSTRACTUNIT_H
#define ABSTRACTUNIT_H

#include <Box2D/Box2D.h>

#include <QSet>

#include <World/Bullet/abstractbullet.h>
class AbstractBullet;

class AbstractUnit
{

public:
    AbstractUnit();
    virtual ~AbstractUnit();

    virtual void moveForward() = 0;
    virtual void moveBack() = 0;

    virtual float getGunAnge() = 0;
    virtual float getBodyAnge() = 0;
    virtual float getXpos() = 0;
    virtual float getYpos() = 0;

    virtual void  setGunAnge(float angle) = 0;
    virtual void  setBodyAnge(float angle) = 0;
    virtual void  setXpos(float x) = 0;
    virtual void  setYpos(float y) = 0;

    virtual int   getHealth() = 0;
    virtual void  setHealth(int hp) = 0;

    virtual int   getId() = 0;
    virtual int   setId(int id) = 0;

    virtual int   getScore() = 0;
    virtual int   setScore(int score) = 0;

    virtual int   getType() = 0;
    virtual int   setType(int type) = 0;

    virtual void destroyUnit() = 0;

    virtual b2Body * getBody() = 0;
    virtual QSet<AbstractBullet *> * getBulletBase() = 0;
};

#endif // ABSTRACTUNIT_H
