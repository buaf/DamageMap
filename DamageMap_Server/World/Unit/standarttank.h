#ifndef STANDARTTANK_H
#define STANDARTTANK_H

#include "World/Unit/abstractunit.h"
class AbstractUnit;

class StandartTank : public AbstractUnit
{
public:
    StandartTank(b2World * world2d, float x_poss, float y_poss, int ownerId);
    virtual ~StandartTank();

    virtual void moveForward();
    virtual void moveBack();

    virtual float getGunAnge();
    virtual float getBodyAnge();
    virtual float getXpos();
    virtual float getYpos();

    virtual void  setGunAnge(float angle);
    virtual void  setBodyAnge(float angle);
    virtual void  setXpos(float x);
    virtual void  setYpos(float y);

    virtual int   getHealth();
    virtual void  setHealth(int hp);

    virtual int   getId();
    virtual int   setId(int id);

    virtual int   getScore();
    virtual int   setScore(int score);

    virtual int   getType();
    virtual int   setType(int type);

    virtual void destroyUnit();

    virtual b2Body * getBody();
    virtual QSet<AbstractBullet *> * getBulletBase();

private:
    b2World * world;

    float gunAngle;
    int health, id, score, type;
    b2Body * tankBody;
    QSet<AbstractBullet *> * bulletBase;
};

#endif // STANDARTTANK_H
