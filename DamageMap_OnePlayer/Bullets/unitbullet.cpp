#include "unitbullet.h"
#include "bodyinfo.h"

#include <math.h>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

UnitBullet::UnitBullet(QObject * parent, qreal bullet_ange, qreal bullet_x_pos, qreal bullet_y_pos, b2World * world2d, AbstractUnit * tank)
    : AbstractBullet(parent) , world2dd(world2d), tankPointer(tank)
{
    isFly = 1;
    b2BodyDef bodyDef;
    bodyDef.userData = new BodyInfo("Bullet", tank->getId(), this);
    bodyDef.active = true;
    //bodyDef.bullet = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(bullet_x_pos, bullet_y_pos);
    bodyDef.angle = bullet_ange;
    bulletBody = world2d->CreateBody(&bodyDef);
    //определяем квадратный квадрат для динамического тела
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    //определяем динамическое тело фикстуру
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    //определяем вес обьекта , если он не равен нулю он динамический
    fixtureDef.density = 1.0f;
    //определяем трение
    fixtureDef.friction = 0.3f;

    fixtureDef.restitution = 1.0f;

    //Добавляем фигуру к телу
    bulletBody->CreateFixture(&fixtureDef);

    qreal bul_ang = bulletBody->GetAngle();
    qreal radians = 0.017453293 * bul_ang;
    b2Vec2 posVec = bulletBody->GetPosition();
    posVec.x = cos(radians)*500;
    posVec.y = sin(radians)*500;
    bulletBody->ApplyLinearImpulse(posVec, bulletBody->GetPosition());

    QTimer::singleShot(4000, this, SLOT(setFlyEnd()));
}

UnitBullet::~UnitBullet()
{
    if(bulletBody->IsActive())
        world2dd->DestroyBody(bulletBody);
}

qreal UnitBullet::get_bul_ang()
{
    return bulletBody->GetAngle();
}

qreal UnitBullet::get_bul_x()
{
    return bulletBody->GetPosition().x;
}

qreal UnitBullet::get_bul_y()
{
    return bulletBody->GetPosition().y;
}

int UnitBullet::getDamageSize()
{
    return 4+qrand()%20;
}

void UnitBullet::setFlyEnd()
{
    isFly = 0;
}

b2Body * UnitBullet::getBody()
{
    return bulletBody;
}

AbstractUnit *UnitBullet::getTankUnit()
{
    return tankPointer;
}

bool UnitBullet::doStep()
{
   if(isFly)
        return 1;
   else
    if(bulletBody->IsActive())
        world2dd->DestroyBody(bulletBody);

    return 0;
}

int UnitBullet::getType()
{
    return 1;
}
