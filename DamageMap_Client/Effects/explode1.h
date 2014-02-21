#ifndef EXPLODE1_H
#define EXPLODE1_H

#include "abstracteffect.h"
#include <QObject>

class Explode1 : public AbstractEffect
{
    Q_OBJECT

private:
    int xPos, yPos, size, effectId, drawCount;
    float xTexPos1, yTexPos1, xTexPos2, yTexPos2;

public:
    Explode1(QObject * parent = 0, int xPos = 10, int yPos = 10, int size = 1, int effectId = 0);
    ~Explode1();
    bool draw();
    int getId();

    static int textudeId;
};

#endif // DOUBLETANK_H
