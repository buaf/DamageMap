#ifndef OGLDRAW_H
#define OGLDRAW_H

#include <QGLWidget>
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include <QDebug>
#include <QWidget>
#include <QMap>

#include "Units/tankunit.h"
#include "Bullets/unitbullet.h"
#include "Effects/abstracteffect.h"
#include "Effects/explode1.h"

#include <Box2D/Box2D.h>
#include "b2debugdraw.h"

class AbstractEffect;

class QGLDraw : public QGLWidget
{
    Q_OBJECT

private:
    int FPS;

    const b2Vec2 * gravity;
    b2World * world;
    DebugDraw * debugDraw;

    GLuint wheelTexture[10];
    GLuint bodyTexture[20];
    GLuint worldTexture[5];


    QTimer * tmDrw;


    QMap<int, AbstractUnit *> * UnitBase;
    QMap<int, AbstractEffect *> * EffectBase;

    int WindowHeight;
    int WindowWidth;
    float scale;

    int startBotCount;


    bool PrsKey[20];


    int toGl_x(int mouse_x);
    int toGl_y(int mouse_y);

public:
    QGLDraw(QWidget * pwgt = 0);

    void loadTextures();


    virtual void initializeGL();
    virtual void resizeGL(int nWidth, int nHeight);
    virtual void paintGL();
    void CalculateFrameRate();


    virtual void keyPressEvent(QKeyEvent * ev);
    virtual void keyReleaseEvent(QKeyEvent * ev);
    virtual void mouseMoveEvent(QMouseEvent * ev);
    virtual void mousePressEvent(QMouseEvent * ev);
    virtual void mouseReleaseEvent(QMouseEvent * ev);
    virtual void wheelEvent(QWheelEvent * ev);

    void add_bot();

    QMap<int,AbstractUnit *> * getUnitBase();
    b2World * getWorld();
};

#endif // OGLDRAW_H
