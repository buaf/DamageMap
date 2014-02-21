#include "bodyinfo.h"
#include <QDebug>

BodyInfo::BodyInfo(QString name, int id)
{
    this->name = name;
    this->id = id;
    this->tankPointer = NULL;
    this->bulletPointer = NULL;
}

BodyInfo::BodyInfo(QString name, int id, AbstractUnit *pointer)
{
    this->name = name;
    this->id = id;
    this->tankPointer = pointer;
    this->bulletPointer = NULL;
}

BodyInfo::BodyInfo(QString name, int id, AbstractBullet *pointer)
{
    this->name = name;
    this->id = id;
    this->bulletPointer = pointer;
    this->tankPointer = NULL;
}

BodyInfo::~BodyInfo()
{
    //delete tankPointer, bulletPointer;
}
