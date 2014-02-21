#ifndef ABSTRACTUNIT_H
#define ABSTRACTUNIT_H

#include <QObject>

#include "Box2D/Box2D.h"
#include "Bullets/abstractbullet.h"
class AbstractBullet;

class AbstractUnit : public QObject
{
    Q_OBJECT

public:
    AbstractUnit(QObject * parent = 0);
    virtual ~AbstractUnit();
    virtual void drawUnit(qreal scale) = 0;
    virtual int setDamage(int num) = 0;
    virtual void setTextures(int wheel_tex, int gun_tex, int head_tex, int body_tex) = 0;
    virtual void rotateGun(qreal num) = 0;
    virtual void rotateBody(qreal num) = 0;
    virtual void moveForward() = 0;
    virtual void moveBack() = 0;
    virtual qreal getxPos() = 0;
    virtual qreal getyPos() = 0;
    virtual qreal getBodyAnge() = 0;
    virtual qreal getGunAnge() = 0;
    virtual int getId() = 0;
    virtual b2Body * getBody() = 0;
    virtual int healthMax() = 0;
    virtual int addScore() = 0;
    virtual QList<AbstractBullet *> * getBulletBase() = 0;
    virtual int getType() = 0;
    virtual int getHealth() = 0;
    int i;

signals:
    
public slots:
    virtual void botGaming() = 0;
    virtual void tankShot() = 0;
    
};

#endif // ABSTRACTUNIT_H
