#ifndef ABSTRACTBULLET_H
#define ABSTRACTBULLET_H

#include <World/Unit/abstractunit.h>
class AbstractUnit;

class AbstractBullet
{

public:
    AbstractBullet();
    virtual ~AbstractBullet();

    virtual float getBulletAngle() = 0;
    virtual float getBulletX() = 0;
    virtual float getBulletY() = 0;

    virtual float destroyBullet() = 0;

    virtual b2Body * getBody() = 0;
    virtual AbstractUnit * getUnit() = 0;

    virtual int getType() = 0;
};

#endif // ABSTRACTBULLET_H
