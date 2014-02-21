#include "ufounit.h"
#include "bodyinfo.h"

#include <math.h>

#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QListIterator>
#include <QTime>

#include <QtOpenGL>
//#include <GL/freeglut.h>

UfoUnit::UfoUnit(QObject *parent, qreal x_poss, qreal y_poss, b2World * world, int tankIDD, QMap<int,AbstractUnit *> * UnitBase) :
    AbstractUnit(parent), gun_ange(qrand()%360), tankID(tankIDD), world2d(world)
{
    this->UnitBase = UnitBase;
    choseNew = 1; //поиск цели включен
    moveORshot = 0; //по умолчанию ехать
    BulletBase = new QList<AbstractBullet *>(); //инициализация контейнера с патронами
    i = 0;
    health = 120;
    score = 0;

    if(!world2d)
    {
        QMessageBox msgBox;
        msgBox.setText("world2d pointer is null!");
        msgBox.exec();
        return;
    }

    b2BodyDef bodyDef;
    bodyDef.userData = new BodyInfo("Tank", tankID, this);
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x_poss, y_poss);
    bodyDef.linearDamping = 1.0f;
    bodyDef.angularDamping = 1.0f;
    tankBody = world2d->CreateBody(&bodyDef);
    b2CircleShape dynamicBox;
    dynamicBox.m_radius = 10.0f; //квадратный
    b2FixtureDef fixtureDef;
    fixtureDef.restitution = 0.0f;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 10.0f; //плотность
    fixtureDef.friction = 0.3f; //коефициент трения
    tankBody->CreateFixture(&fixtureDef);
}

UfoUnit::~UfoUnit()
{
    QListIterator<AbstractBullet*> it(*BulletBase);
    while(it.hasNext())
    {
        world2d->DestroyBody(it.next()->getBody());
    }

    //qDebug() << "TankUnit::~TankUnit()";
    BulletBase->clear();
    delete BulletBase;
}

void UfoUnit::drawUnit(qreal scale)
{

    //поворот и перемещение
    glLoadIdentity();
    glTranslatef(tankBody->GetPosition().x, tankBody->GetPosition().y, 0);

    char ch[24];
    sprintf(ch, "Health : %d Score : %d", health, score);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(tankBody->GetPosition().x-50/scale, tankBody->GetPosition().y+30);
    for (int32 i = 0; i < 24; ++i)
    {
        //glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ch[i]);
    }


    if(tankID == 0) //что бы не мерять расстояние к самому себе
    {
    char cch[16];
    sprintf(cch, "Tank count : %d", UnitBase->size());
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(getxPos()-380/scale, getyPos()+370);
       for (int32 i = 0; i < 16; ++i)
       {
        //glutBitmapCharacter(GLUT_BITMAP_8_BY_13, cch[i]);
       }
    }

    glPopMatrix();
    glRotatef(tankBody->GetAngle()*(180/3.1415926535f)+90, 0, 0, 1);
    glTranslatef(0,0,0);

    glEnable(GL_BLEND);        // Включаем смешивание
    glDisable(GL_DEPTH_TEST);

    //отрисовка тела
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bodyTextureId);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
       glVertex3f(-10.0f, 10.0f, 0.0f);
       glTexCoord2f(1.0f, 0.0f);
       glVertex3f( 10.0f, 10.0f, 0.0f);
       glTexCoord2f(1.0f, 1.0f);
       glVertex3f( 10.0f,-10.0f, 0.0f);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(-10.0f,-10.0f, 0.0f);
    glEnd();
    //-----------------------------------------------------------

    //поворот и перемещение головы
    glLoadIdentity();
    glTranslatef(tankBody->GetPosition().x, tankBody->GetPosition().y,0);
    glRotatef(gun_ange+90, 0, 0, 1);
    glTranslatef(0,0,0);


    //пушка
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gunTextureId);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin( GL_POLYGON);
       glTexCoord2f(0.0f, 0.0f);
       glVertex3f(-3.5f, 3.5f, 0.0f);
       glTexCoord2f(1.0f, 0.0f);
       glVertex3f( 3.5f, 3.5f, 0.0f);
       glTexCoord2f(1.0f, 1.0f);
       glVertex3f( 3.5f,-9.0f, 0.0f);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(-3.5f,-9.0f, 0.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);


    for(int j=0; j<BulletBase->size(); j++)
    {
        if(!BulletBase->value(j)->doStep())
        {
            BulletBase->removeAt(j);
            continue;
        }
        glLoadIdentity();
        glTranslatef(BulletBase->value(j)->get_bul_x(), BulletBase->value(j)->get_bul_y(), 0);
        glRotatef(BulletBase->value(j)->get_bul_ang(), 0, 0, 1);
        glTranslatef(0,0,0);

        //рисует пулю
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);
        glEnd();
    }
}


void UfoUnit::rotateGun(qreal num)
{
    if(gun_ange > 360 || gun_ange < -360) //защита от перекрутки угла
        gun_ange = 0;  //поворот орудия
    gun_ange = num;
}

void UfoUnit::rotateBody(qreal num)
{
    qreal body_ange = tankBody->GetAngle() + num;

    if(std::abs(body_ange) > 6.28318530718)
        body_ange = 0;

    tankBody->SetTransform(tankBody->GetPosition(), body_ange);
}

void UfoUnit::moveForward()
{
    qreal ange = tankBody->GetAngle();
    b2Vec2 vec = tankBody->GetPosition();
    vec.Set(vec.x + cos(ange)*1.6, vec.y + sin(ange)*1.3);
    tankBody->SetTransform(vec, tankBody->GetAngle());
}

void UfoUnit::moveBack()
{
    qreal ange = tankBody->GetAngle();
    b2Vec2 vec = tankBody->GetPosition();
    vec.Set(vec.x - cos(ange), vec.y - sin(ange));
    tankBody->SetTransform(vec, tankBody->GetAngle());
}

void UfoUnit::tankShot()
{
    if(BulletBase->size() > 20)
        return;
    i++;
    if(i < 50)
        return;
    else
        i = 0;

    qreal radians = 0.017453293 * gun_ange;
    AbstractBullet * bulls = new UnitBullet(NULL, gun_ange, tankBody->GetPosition().x+cos(radians)*17, tankBody->GetPosition().y+sin(radians)*17, world2d, this); //пуля стартует из пушки танка
    BulletBase->append(bulls); //возвращает экземпляр пули уже соидененной со столом для постоянного перемещения
}

qreal UfoUnit::getxPos()
{
    return tankBody->GetPosition().x;
}

qreal UfoUnit::getyPos()
{
    return tankBody->GetPosition().y;
}

qreal UfoUnit::getBodyAnge()
{
    return tankBody->GetAngle();
}

qreal UfoUnit::getGunAnge()
{
    return gun_ange;
}

int UfoUnit::getId()
{
    return tankID;
}

b2Body * UfoUnit::getBody()
{
    return tankBody;
}

int UfoUnit::addScore()
{
    return ++score;
}

int UfoUnit::healthMax()
{
    return 60;
}

int UfoUnit::setDamage(int num)
{
    health -= num;
    return health;
}

void UfoUnit::setTextures(int wheel_tex, int gun_tex, int head_tex, int body_tex)
{

}

QList<AbstractBullet *> * UfoUnit::getBulletBase()
{
    return BulletBase;
}


void UfoUnit::botGaming()
{
    int length = 10000;

    if(choseNew)
    {
       foreach(AbstractUnit * tnk, UnitBase->values())
       {
           int len = std::sqrt(std::pow((tankBody->GetPosition().x - tnk->getxPos()), 2) + std::pow((tankBody->GetPosition().y - tnk->getyPos()), 2));
           //qDebug() << "Length to enemy:" <<len << " " << "Id choosed" << choosedID << " " ;

           if(len < length && len > 2)
           {
               length = len;
               choosedID = tnk->getId();
           }
       }
       choseNew = 0;
    }
    else
    {
        if(!UnitBase->contains(choosedID))
        {
            choseNew = 1;
            return;
        }

        if(!moveORshot) // если 0 значит двигатся
        {
        angeMove = std::atan2(UnitBase->value(choosedID)->getyPos() - tankBody->GetPosition().y, UnitBase->value(choosedID)->getxPos() - tankBody->GetPosition().x);
        tankBody->SetTransform(tankBody->GetPosition(), angeMove);
        moveForward();

        int len = std::sqrt(std::pow((tankBody->GetPosition().x - UnitBase->value(choosedID)->getxPos()), 2) + std::pow((tankBody->GetPosition().y - UnitBase->value(choosedID)->getyPos()), 2));

        //qDebug() << "Length to enemy:" <<len;

           if(len < 500)
               moveORshot = 1;


        }
        else
        {
            int len = std::sqrt(std::pow((tankBody->GetPosition().x - UnitBase->value(choosedID)->getxPos()), 2) + std::pow((tankBody->GetPosition().y - UnitBase->value(choosedID)->getyPos()), 2));
            if(len > 500)
                moveORshot = 0;

            gun_ange = std::atan2(UnitBase->value(choosedID)->getyPos() - tankBody->GetPosition().y, UnitBase->value(choosedID)->getxPos() - tankBody->GetPosition().x);
            gun_ange = gun_ange * (180 / 3.14);

            tankShot();
        }
    }

}

int UfoUnit::getType()
{
    return 1;
}

int UfoUnit::getHealth()
{
    return health;
}

