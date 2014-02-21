#ifndef ABSTRACTBULLET_H
#define ABSTRACTBULLET_H

#include <QObject>
#include <Units/abstractunit.h>
class AbstractUnit;

class AbstractBullet : public QObject
{
    Q_OBJECT
public:
    AbstractBullet(QObject *parent = 0);
    virtual ~AbstractBullet();
    virtual qreal get_bul_ang() = 0;
    virtual qreal get_bul_x() = 0;
    virtual qreal get_bul_y() = 0;
    virtual int getDamageSize() = 0;
    virtual b2Body * getBody() = 0;
    virtual AbstractUnit * getTankUnit() = 0;
    virtual bool doStep() = 0;
    virtual int getType() = 0;

signals:
    
public slots:
    virtual void setFlyEnd() = 0;
};

#endif // ABSTRACTBULLET_H
