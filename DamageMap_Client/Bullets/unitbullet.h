#ifndef UNITBULLET_H
#define UNITBULLET_H

#include <QObject>
#include "Units/tankunit.h"
#include "Bullets/abstractbullet.h"

class b2World;
class b2Body;
class TankUnit;

//класс который будет отвечать за координаты патрона который будет выпускать танк
class UnitBullet : public AbstractBullet
{
    Q_OBJECT

public:
    UnitBullet(QObject * parent = 0, qreal bullet_ange = 0, qreal bullet_x_pos = 0, qreal bullet_y_pos = 0, b2World * world2d = 0, AbstractUnit * tank = 0);
    ~UnitBullet();
    //взятие координат и угла
    qreal get_bul_ang();
    qreal get_bul_x();
    qreal get_bul_y();
    int getDamageSize();
    b2Body * getBody();
    AbstractUnit * getTankUnit();
    bool doStep(); //возвращает 0 когда пулю пора удалить
    int getType();

private:
    bool isFly; //живая ли пуля ?
    b2World * world2dd; //указатель на графический мир дла подальшей с ним роботы
    b2Body * bulletBody; //указатель на тело пули
    AbstractUnit * tankPointer; // указатель на танк который выпустил пулю

signals:
    //-------------

public slots:
    void setFlyEnd(); //останавливает пулю
};

#endif // UNITBULLET_H
