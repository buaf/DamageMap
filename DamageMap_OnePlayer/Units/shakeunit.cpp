#include "shakeunit.h"
#include "bodyinfo.h"

#include <math.h>

#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QListIterator>
#include <QTime>

#include <QtOpenGL>
#include <GL/freeglut.h>

ShakeUnit::ShakeUnit(QObject *parent, qreal x_poss, qreal y_poss, b2World * world, int tankIDD, QMap<int,AbstractUnit *> * UnitBase) :
    AbstractUnit(parent), gun_ange(qrand()%360), tankID(tankIDD), world2d(world)
{
    this->UnitBase = UnitBase;
    choseNew = 1; //поиск цели включен
    moveORshot = 0; //по умолчанию ехать
    BulletBase = new QList<AbstractBullet *>(); //инициализация контейнера с патронами

    health = 1000;
    score = 0;

    if(!world2d)
    {
        QMessageBox msgBox;
        msgBox.setText("world2d pointer is null!");
        msgBox.exec();
        return;
    }

    for(int i=0; i<10; i++)
    {
    b2BodyDef bodyDef;
    bodyDef.userData = new BodyInfo("Shake", tankID, this);
    bodyDef.type = b2_dynamicBody;
    bodyDef.angle = 3.14;
    bodyDef.position.Set(x_poss, y_poss);
    tankBody[i] = world2d->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(10.0f, 10.0f); //квадратный
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f; //плотность
    fixtureDef.friction = 0.3f; //коефициент трения
    tankBody[i]->CreateFixture(&fixtureDef);
       if(i)
       {
          b2RevoluteJointDef jointDef1;
          jointDef1.bodyA = tankBody[i-1]; //первое тело соединения
          jointDef1.bodyB = tankBody[i]; //второе тело соединения
          jointDef1.collideConnected = false; //тела не сталкиваются
          jointDef1.localAnchorA = b2Vec2(-6, 0); //якорная точка первого тела
          jointDef1.localAnchorB = b2Vec2(6, 0); //якорная точка второго тела
          //jointDef1.enableLimit = true; //включаем пределы
          jointDef1.upperAngle=0;
          jointDef1.referenceAngle = 0; //начальный угол соединения
          world2d->CreateJoint(&jointDef1);
       }
       else
           bodyAnge = tankBody[0]->GetAngle();
    }

}

ShakeUnit::~ShakeUnit()
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

void ShakeUnit::drawUnit(qreal scale)
{
    //эмуляция создание нового танка
    if(health < 1)
    {
        int x = (qrand()%500 + (0-qrand()%500));
        int y = (qrand()%500 + (0-qrand()%500));

        for(int i=0; i<10; i++)
           tankBody[i]->SetTransform(b2Vec2(x, y), 0);
        health = this->healthMax();

    }

    char ch[24];
    sprintf(ch, "Health : %d Score : %d", health, score);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(tankBody[0]->GetPosition().x-50/scale, tankBody[0]->GetPosition().y+30);
    for (int32 i = 0; i < 24; ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ch[i]);
    }
    if(tankID == 0) //что бы не мерять расстояние к самому себе
    {
    char cch[16];
    sprintf(cch, "Tank count : %d", UnitBase->size());
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(getxPos()-380/scale, getyPos()+370);
       for (int32 i = 0; i < 16; ++i)
       {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, cch[i]);
       }
    }
    glPopMatrix();

//------000000000--------------------------------------------------------------------
    for(int i=0; i<10; i++)
    {
        glLoadIdentity();
        glTranslatef(tankBody[i]->GetPosition().x, tankBody[i]->GetPosition().y, 0);
        glRotatef((tankBody[i]->GetAngle()*(180/3.1415926535f))+90, 0, 0, 1);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if(i != 0 && i !=9) //тело
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, shakeTexture[1]);
            glColor3f(1.0f, 1.0f, 1.0f);

            glBegin(GL_QUADS);
               glTexCoord2f(0.0f, 0.0f);
               glVertex3f(9.0f, 9.0f, 0.0f);//
               glTexCoord2f(1.0f, 0.0f);
               glVertex3f( -9.0f, 9.0f, 0.0f);//
               glTexCoord2f(1.0f, 1.0f);
               glVertex3f( -9.0f, -9.0f, 0.0f);//
               glTexCoord2f(0.0f, 1.0f);
               glVertex3f(9.0f, -9.0f, 0.0f);//
            glEnd();
        }
        else if(i == 9) //хвост
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, shakeTexture[2]);
            glColor3f(1.0f, 1.0f, 1.0f);

            glBegin(GL_QUADS);
               glTexCoord2f(0.0f, 0.0f);
               glVertex3f(9.0f, 9.0f, 0.0f);//
               glTexCoord2f(1.0f, 0.0f);
               glVertex3f( -9.0f, 9.0f, 0.0f);//
               glTexCoord2f(1.0f, 1.0f);
               glVertex3f( -9.0f, -9.0f, 0.0f);//
               glTexCoord2f(0.0f, 1.0f);
               glVertex3f(9.0f, -9.0f, 0.0f);//
            glEnd();
        }
        else //голова
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, shakeTexture[0]);
            glColor3f(1.0f, 1.0f, 1.0f);

            glBegin(GL_QUADS);
               glTexCoord2f(0.0f, 0.0f);
               glVertex3f(9.0f, 9.0f, 0.0f);//
               glTexCoord2f(1.0f, 0.0f);
               glVertex3f( -9.0f, 9.0f, 0.0f);//
               glTexCoord2f(1.0f, 1.0f);
               glVertex3f( -9.0f, -9.0f, 0.0f);//
               glTexCoord2f(0.0f, 1.0f);
               glVertex3f(9.0f, -9.0f, 0.0f);//
            glEnd();
        }
        /*glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

    }
}

void ShakeUnit::setTextures(int, int tail_tex, int head_tex, int body_tex)
{
    shakeTexture[0]=head_tex; shakeTexture[1]=body_tex; shakeTexture[2]=tail_tex;
}

void ShakeUnit::rotateGun(qreal num)
{

}

void ShakeUnit::rotateBody(qreal num)
{
    qreal body_ange = tankBody[0]->GetAngle() + num;
    tankBody[0]->SetTransform(tankBody[0]->GetPosition(), body_ange);
}

void ShakeUnit::moveForward()
{
    for(int i=0; i<10; i++)
    {
    qreal ange = tankBody[i]->GetAngle();
    b2Vec2 vec = tankBody[i]->GetPosition();
    vec.Set(vec.x + cos(ange), vec.y + sin(ange));
    tankBody[i]->SetTransform(vec, tankBody[i]->GetAngle());
    }
}

void ShakeUnit::moveBack()
{
    for(int i=0; i<10; i++)
    {
    qreal ange = tankBody[i]->GetAngle();
    b2Vec2 vec = tankBody[i]->GetPosition();
    vec.Set(vec.x - cos(ange)/2, vec.y - sin(ange)/2);
    tankBody[i]->SetTransform(vec, tankBody[i]->GetAngle());
    }
}

void ShakeUnit::tankShot()
{

}

qreal ShakeUnit::getxPos()
{
    return tankBody[0]->GetPosition().x;
}

qreal ShakeUnit::getyPos()
{
    return tankBody[0]->GetPosition().y;
}

qreal ShakeUnit::getBodyAnge()
{
    return tankBody[0]->GetAngle();
}

qreal ShakeUnit::getGunAnge()
{
    return gun_ange;
}

int ShakeUnit::getId()
{
    return tankID;
}

b2Body * ShakeUnit::getBody()
{
    return tankBody[0];
}

int ShakeUnit::addScore()
{
    return ++score;
}

int ShakeUnit::healthMax()
{
    return 1000;
}

int ShakeUnit::setDamage(int num)
{
    health -= num;
    return health;
}

QList<AbstractBullet *> * ShakeUnit::getBulletBase()
{
    return BulletBase;
}

void ShakeUnit::botGaming()
{
    int length = 10000;

    if(choseNew)
    {
       foreach(AbstractUnit * tnk, UnitBase->values())
       {
           int len = std::sqrt(std::pow((tankBody[0]->GetPosition().x - tnk->getxPos()), 2) + std::pow((tankBody[0]->GetPosition().y - tnk->getyPos()), 2));
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

        angeMove = std::atan2(UnitBase->value(choosedID)->getyPos() - tankBody[0]->GetPosition().y, UnitBase->value(choosedID)->getxPos() - tankBody[0]->GetPosition().x);
        tankBody[0]->SetTransform(tankBody[0]->GetPosition(), angeMove);
        moveForward();
    }

}

int ShakeUnit::getType()
{
    return 3;
}
