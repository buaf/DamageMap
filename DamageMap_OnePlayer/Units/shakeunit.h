#ifndef SHAKEUNIT_H
#define SHAKEUNIT_H

#include "Units/abstractunit.h"
#include "Bullets/abstractbullet.h"

class ShakeUnit : public AbstractUnit
{
    Q_OBJECT
public:
    ShakeUnit(QObject * parent = 0, qreal x_poss = 0, qreal y_poss = 0,b2World * world = NULL, int tankIDD = 0, QMap<int,AbstractUnit *> * UnitBase = NULL);
    ~ShakeUnit();
    void drawUnit(qreal scale); //отрисовка юнита
    int setDamage(int num); //нанести урон
    void setTextures(int, int tail_tex, int head_tex, int body_tex); //установка текстур
    //повороты и движения
    void rotateGun(qreal num);
    void rotateBody(qreal num);
    void moveForward();
    void moveBack();
    //возвращает позицию и углы
    qreal getxPos();
    qreal getyPos();
    qreal getBodyAnge();
    qreal getGunAnge();
    int getId(); //возвращает id
    b2Body * getBody(); //возвращает указатель на тело
    int addScore(); //добавить +1 в счет
    int healthMax();
    QList<AbstractBullet *> * getBulletBase();
    int getType();

private:
    qreal bodyAnge;
    qreal gun_ange; //угол пушки

    int shakeTexture[3];
    int health; //оставшиеся жизни танка
    int score; //число убитых
    int shotTimeout; //время до следующего выстрела
    int tankID; //id танка

    bool choseNew; //определяет выбор цели для бота
    bool moveORshot; //стрелять ихи ехать
    int choosedID;
    qreal angeMove;

    b2World * world2d; //указатель на графический мир дла подальшей с ним роботы
    b2Body * tankBody[10]; //указатель на тело танка
    QMap<int,AbstractUnit *> * UnitBase; //указатель на основную базу юнитов
    QList<AbstractBullet *> * BulletBase; //пули выстреленые танком,для кадого танка своя база

public slots:
    void botGaming(); //движение по кругу
    void tankShot();//вовзращает обьект пули из текущего танка
};

#endif // SHAKEUNIT_H
