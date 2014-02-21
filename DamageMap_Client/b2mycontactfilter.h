#ifndef B2MYCONTACTFILTER_H
#define B2MYCONTACTFILTER_H

#include "Effects/abstracteffect.h"
#include "Effects/explode1.h"
#include <Box2D/Box2D.h>
#include <QMap>


class b2MyContactFilter : public  b2ContactFilter
{
private:
    QMap<int, AbstractEffect *> * EffectBase;

public:
    b2MyContactFilter(QMap<int, AbstractEffect *> * EffectBase = 0);
    virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
};

#endif // B2MYCONTACTFILTER_H
