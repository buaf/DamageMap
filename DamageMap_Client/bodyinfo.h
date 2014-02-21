#ifndef BODYINFO_H
#define BODYINFO_H

#include <QString>

#include "Units/tankunit.h"
#include "Bullets/unitbullet.h"

//класс оболочка для обьектов

class BodyInfo
{
public:
    BodyInfo(QString name, int id);
    BodyInfo(QString name, int id, AbstractUnit * pointer);
    BodyInfo(QString name, int id, AbstractBullet * pointer);
    ~BodyInfo();

    QString name;
    int id;
    AbstractUnit * tankPointer;
    AbstractBullet * bulletPointer;
};

#endif // BODYINFO_H
