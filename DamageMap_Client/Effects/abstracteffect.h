#ifndef ABSTRACEFFECT_H
#define ABSTRACEFFECT_H

#include <QObject>

class AbstractEffect : public QObject
{
    Q_OBJECT
public:
    AbstractEffect(QObject * parent = 0);
    virtual ~AbstractEffect();
    virtual bool draw() = 0;
    virtual int getId() = 0;
};

#endif // ABSTRACTUNIT_H
