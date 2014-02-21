#include "doubletank.h"
#include "bodyinfo.h"

#include <math.h>

#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QListIterator>
#include <QTime>

#include <QtOpenGL>
//#include <GL/freeglut.h>

DoubleTank::DoubleTank(QObject *parent, qreal x_poss, qreal y_poss, b2World * world, int tankIDD, QMap<int,AbstractUnit *> * UnitBase) :
    AbstractUnit(parent), gun_ange(qrand()%360), tankID(tankIDD), world2d(world)
{
    wheelPos = 1;
    i = 0;
    this->UnitBase = UnitBase;
    choseNew = 1; //поиск цели включен
    moveORshot = 0; //по умолчанию ехать
    BulletBase = new QList<AbstractBullet *>(); //инициализация контейнера с патронами

    health = 200;
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
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(11.0f, 11.0f); //квадратный
    b2FixtureDef fixtureDef;
    fixtureDef.restitution = 0.0f;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 15.0f; //плотность
    fixtureDef.friction = 0.3f; //коефициент трения
    tankBody->CreateFixture(&fixtureDef);
}

DoubleTank::~DoubleTank()
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

void DoubleTank::drawUnit(qreal scale)
{
    //эмуляция создание нового танка
    /*if(health < 1)
    {
        getBody()->SetTransform(b2Vec2((qrand()%500 + (0-qrand()%500)),(qrand()%500 + (0-qrand()%500))), qrand()%360);
        health = this->healthMax();
    }*/


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
    glRotatef(tankBody->GetAngle()*(180/3.1415926535f), 0, 0, 1);
    glTranslatef(0,0,0);


    //текстура колес
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wheel_tex);
    glColor3f(1.0f, 1.0f, 1.0f);



    //левое колесо
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-11.0f, -5.0f, 0.0f);
    glTexCoord2f(1.0f + wheelPos, 0.0f);
    glVertex3f( 11.0f, -5.0f, 0.0f);
    glTexCoord2f(1.0f + wheelPos, 1.0f + wheelPos);
    glVertex3f( 11.0f, -11.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f + wheelPos);
    glVertex3f(-11.0f, -11.0f, 0.0f);
    glEnd();

    //правое колесо
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-11.0f, 5.0f, 0.0f);
    glTexCoord2f(1.0f + wheelPos, 0.0f);
    glVertex3f( 11.0f, 5.0f, 0.0f);
    glTexCoord2f(1.0f + wheelPos, 1.0f + wheelPos);
    glVertex3f( 11.0f, 11.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f + wheelPos);
    glVertex3f(-11.0f, 11.0f, 0.0f);
    glEnd();

    //основа
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, body_tex);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-8.0f, 5.0f, 0.0f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f( 8.0f, 5.0f, 0.0f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f( 8.0f,-5.0f, 0.0f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-8.0f,-5.0f, 0.0f);
    glEnd();
    //-----------------------------------------------------------

    //поворот и перемещение головы
    glLoadIdentity();
    glTranslatef(tankBody->GetPosition().x, tankBody->GetPosition().y,0);
    glRotatef(gun_ange, 0, 0, 1);
    glTranslatef(0,0,0);

    //голова
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, head_tex);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin( GL_POLYGON);
       glTexCoord2f(1.0f,1.0f);
       glVertex2f(7.0f,7.0f);

       glTexCoord2f(1.0f,-1.0f);
       glVertex2f(7.0f,-7.0f);

       glTexCoord2f(-1.0f,-1.0f);
       glVertex2f(-7.0f,-7.0f);

       glTexCoord2f(-1.0f,1.0f);
       glVertex2f(-7.0f,7.0f);
    glEnd();

    //дуло----------------------------------------
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gun_tex);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 0.0f);
       glVertex3f(15.0f, 3.0f, 0.0f);

       glTexCoord2f(1.0f, 0.0f);
       glVertex3f( 7.0f, 3.0f, 0.0f);

       glTexCoord2f(1.0f, 1.0f);
       glVertex3f( 7.0f, 1.0f, 0.0f);

       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(15.0f, 1.0f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 0.0f);
       glVertex3f(15.0f, -3.0f, 0.0f);

       glTexCoord2f(1.0f, 0.0f);
       glVertex3f( 7.0f, -3.0f, 0.0f);

       glTexCoord2f(1.0f, 1.0f);
       glVertex3f( 7.0f,-1.0f, 0.0f);

       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(15.0f,-1.0f, 0.0f);
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

void DoubleTank::setTextures(int wheel_tex, int gun_tex, int head_tex, int body_tex)
{
    this->wheel_tex = wheel_tex;
    this->gun_tex = gun_tex;
    this->head_tex = head_tex;
    this->body_tex = body_tex;
}

void DoubleTank::rotateGun(qreal num)
{
    if(gun_ange > 360 || gun_ange < -360) //защита от перекрутки угла
        gun_ange = 0;  //поворот орудия
    gun_ange = num;
}

void DoubleTank::rotateBody(qreal num)
{
    if(++wheelPos > 5)
        wheelPos = 0;

    qreal body_ange = tankBody->GetAngle() + num;

    if(std::abs(body_ange) > 6.28318530718)
        body_ange = 0;

    tankBody->SetTransform(tankBody->GetPosition(), body_ange);
}

void DoubleTank::moveForward()
{
    if(++wheelPos > 5)
        wheelPos = 0;

    qreal ange = tankBody->GetAngle();
    b2Vec2 vec = tankBody->GetPosition();
    vec.Set(vec.x + cos(ange)*0.8, vec.y + sin(ange)*0.8);
    tankBody->SetTransform(vec, tankBody->GetAngle());
}

void DoubleTank::moveBack()
{
    if(++wheelPos > 5)
        wheelPos = 0;

    qreal ange = tankBody->GetAngle();
    b2Vec2 vec = tankBody->GetPosition();
    vec.Set(vec.x - cos(ange)*0.8, vec.y - sin(ange)*0.8);
    tankBody->SetTransform(vec, tankBody->GetAngle());
}

void DoubleTank::tankShot()
{
    if(BulletBase->size() > 20)
        return;

    i++;
    if(i < 30)
        return;
    else
        i = 0;

    qreal radians = 0.017453293 * gun_ange;
    AbstractBullet * bulls1 = new UnitBullet(NULL, gun_ange, tankBody->GetPosition().x-1+cos(radians)*17, tankBody->GetPosition().y-1+sin(radians)*17, world2d, this); //пуля стартует из пушки танка
    AbstractBullet * bulls2 = new UnitBullet(NULL, gun_ange, tankBody->GetPosition().x+1+cos(radians)*17, tankBody->GetPosition().y+1+sin(radians)*17, world2d, this); //пуля стартует из пушки танка
    BulletBase->append(bulls1);
    BulletBase->append(bulls2);
}

qreal DoubleTank::getxPos()
{
    return tankBody->GetPosition().x;
}

qreal DoubleTank::getyPos()
{
    return tankBody->GetPosition().y;
}

qreal DoubleTank::getBodyAnge()
{
    return tankBody->GetAngle();
}

qreal DoubleTank::getGunAnge()
{
    return gun_ange;
}

int DoubleTank::getId()
{
    return tankID;
}

b2Body * DoubleTank::getBody()
{
    return tankBody;
}

int DoubleTank::addScore()
{
    return ++score;
}

int DoubleTank::healthMax()
{
    return 200;
}

int DoubleTank::setDamage(int num)
{
    health -= num;
    return health;
}

QList<AbstractBullet *> * DoubleTank::getBulletBase()
{
    return BulletBase;
}

void DoubleTank::botGaming()
{
    int length = 10000;

    if(choseNew)
    {
       foreach(AbstractUnit * tnk, UnitBase->values())
       {
           int len = std::sqrt(std::pow((tankBody->GetPosition().x - tnk->getxPos()), 2) + std::pow((tankBody->GetPosition().y - tnk->getyPos()), 2));
           ////qDebug() << "Length to enemy:" <<len << " " << "Id choosed" << choosedID << " " ;

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

int DoubleTank::getType()
{
    return 2;
}

int DoubleTank::getHealth()
{
    return health;
}
