#include "glpaintscene.h"

#include <QTime>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <QTime>
#include <QCursor>
#include <QBitmap>
#include <QMessageBox>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include <GL/freeglut.h>

#include "b2mycontactfilter.h"
#include "bodyinfo.h"

#include "Units/doubletank.h"
#include "Units/bmpunit.h"
#include "Units/ufounit.h"
#include "Units/carunit.h"

int Explode1::textudeId = 0;
int BmpUnit::bodyTextureId = 0;
int BmpUnit::gunTextureId = 0;
int UfoUnit::bodyTextureId = 0;
int UfoUnit::gunTextureId = 0;
int CarUnit::bodyTextureId = 0;
int CarUnit::gunTextureId = 0;

//----преобразование координат окна в координаты OpenGL
int QGLDraw::toGl_x(int mouse_x)
{
    return (mouse_x - (WindowWidth / 2)) / 4;
}

int QGLDraw::toGl_y(int mouse_y)
{
    return -((mouse_y - (WindowHeight / 2)) / 4);
}



// ----------------------------------------------------------------------
QGLDraw::QGLDraw(QWidget * pwgt, gameInfo * info) : QGLWidget(pwgt)
{    
    setCursor(QCursor(QPixmap("Texture/Cross.png")));

    EffectBase = new QMap<int, AbstractEffect *>();
    EffectBase->insert(0,new Explode1());


    isClientMode = 0;
    if(info->gameMode == 2)
        isClientMode = 1;

    gravity = new b2Vec2(0.0f, 0.0f);
    world = new b2World(*gravity);
    debugDraw = new DebugDraw();
    world->SetDebugDraw(debugDraw);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_centerOfMassBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    debugDraw->SetFlags(flags);
    world->SetAllowSleeping(0); //отключили сон
    b2MyContactFilter * filter = new b2MyContactFilter(EffectBase); //наш переопределенный контакт фильтер для столкновений
    world->SetContactFilter(filter);
    setMouseTracking(true);

    UnitBase = new QMap<int,AbstractUnit *>();

    scale = 2;

    for(int i = 0;i < 20;i++) //инициализировали массив перемещения нулями (все клапиши отпущены)
        PrsKey[i] = 0;

    //ДОБАВЛЕНИЕ СЛУЧАЙНОГО ЮНИТА ИГРОКУ-------------------------------
    qsrand(QTime::currentTime().msec());
    int rand = qrand()%5;

    //qDebug() << "Create=" <<rand;

    switch(rand)
    {
    case 0:
    {
       AbstractUnit * reg = new TankUnit(NULL, 0, 0, world, 0, UnitBase);
       UnitBase->insert(0, reg);
    }
    break;

    case 1:
    {
       AbstractUnit * reg = new DoubleTank(NULL, 0, 0, world, 0, UnitBase);
       UnitBase->insert(0, reg);
    }
    break;

    case 2:
    {
       AbstractUnit * reg = new CarUnit(NULL, 0, 0, world, 0, UnitBase);
       UnitBase->insert(0, reg);
    }
    break;

    case 3:
    {
       AbstractUnit * reg = new BmpUnit(NULL, 0, 0, world, 0, UnitBase);
       UnitBase->insert(0, reg);
    }
    break;

    case 4:
    {
       AbstractUnit * reg = new UfoUnit(NULL, 0, 0, world, 0, UnitBase);
       UnitBase->insert(0, reg);
    }
    break;
    }
    //ДОБАВЛЕНИЕ СЛУЧАЙНОГО ЮНИТА ИГРОКУ-------------------------------

    //AbstractUnit * reg = new CarUnit(NULL, 0, 0, world, 0, UnitBase);
    //UnitBase->insert(0, reg);

    tmDrw = new QTimer;
    connect(tmDrw,SIGNAL(timeout()),SLOT(updateGL()));
    tmDrw->start(13); //будет 60-80 кадров в секунду


    /*b2BodyDef groundBodyDef1;
    groundBodyDef1.userData = new BodyInfo("Construction", 0);
    groundBodyDef1.position.Set(0, 0);
    b2Body* groundBody1 = world->CreateBody(&groundBodyDef1);
    b2PolygonShape groundBox1;
    groundBox1.SetAsBox(10+qrand()%100, 10+qrand()%100);
    groundBody1->CreateFixture(&groundBox1, 0.0f);*/

    b2BodyDef groundBodyDef2;
    groundBodyDef2.userData = new BodyInfo("Construction", 0);
    groundBodyDef2.position.Set(0, 1000);
    b2Body* groundBody2 = world->CreateBody(&groundBodyDef2);
    b2PolygonShape groundBox2;
    groundBox2.SetAsBox(1000, 2);
    groundBody2->CreateFixture(&groundBox2, 0.0f);

    b2BodyDef groundBodyDef3;
    groundBodyDef3.userData = new BodyInfo("Construction", 0);
    groundBodyDef3.position.Set(0, -1000);
    b2Body* groundBody3 = world->CreateBody(&groundBodyDef3);
    b2PolygonShape groundBox3;
    groundBox3.SetAsBox(1000, 2);
    groundBody3->CreateFixture(&groundBox3, 0.0f);

    b2BodyDef groundBodyDef4;
    groundBodyDef4.userData = new BodyInfo("Construction", 0);
    groundBodyDef4.position.Set(1000, 0);
    b2Body* groundBody4 = world->CreateBody(&groundBodyDef4);
    b2PolygonShape groundBox4;
    groundBox4.SetAsBox(2, 1000);
    groundBody4->CreateFixture(&groundBox4, 0.0f);

    b2BodyDef groundBodyDef5;
    groundBodyDef5.userData = new BodyInfo("Construction", 0);
    groundBodyDef5.position.Set(-1000, 0);
    b2Body* groundBody5 = world->CreateBody(&groundBodyDef5);
    b2PolygonShape groundBox5;
    groundBox5.SetAsBox(2, 1000);
    groundBody5->CreateFixture(&groundBox5, 0.0f);

        if(info->isFullScreen)
            qDebug() << "Load in full screen mode..";
        else
            qDebug() << "Display size : " << info->height << " X " << info->width;
}

void QGLDraw::loadTextures()
{
    for(int i=0; i<20; i++)
    {
        bodyTexture[i]=bindTexture(QPixmap(QString("Texture/body%1.jpg").arg(i+1)), GL_TEXTURE_2D);
    }

    for(int i=0; i<5; i++)
    {
        wheelTexture[i]=bindTexture(QPixmap(QString("Texture/wheel%1.jpg").arg(i+1)), GL_TEXTURE_2D);
    }

    worldTexture[0]=bindTexture(QPixmap(QString("Texture/world%1.jpg").arg(1)), GL_TEXTURE_2D);

    Explode1::textudeId = bindTexture(QPixmap(QString("Texture/Effects/explode_1.png")), GL_TEXTURE_2D);

    BmpUnit::bodyTextureId = bindTexture(QPixmap(QString("Texture/Units/bmp_body.png")), GL_TEXTURE_2D);
    BmpUnit::gunTextureId = bindTexture(QPixmap(QString("Texture/Units/bmp_gun.png")), GL_TEXTURE_2D);

    UfoUnit::bodyTextureId = bindTexture(QPixmap(QString("Texture/Units/ufo_body.png")), GL_TEXTURE_2D);
    UfoUnit::gunTextureId = bindTexture(QPixmap(QString("Texture/Units/ufo_gun.png")), GL_TEXTURE_2D);

    CarUnit::bodyTextureId = bindTexture(QPixmap(QString("Texture/Units/car_body.png")), GL_TEXTURE_2D);
    CarUnit::gunTextureId = bindTexture(QPixmap(QString("Texture/Units/car_gun.png")), GL_TEXTURE_2D);
}

// ----------------------------------------------------------------------
void QGLDraw::initializeGL()
{
    qglClearColor(Qt::black); //цвет очистки фона
    loadTextures();

    UnitBase->value(0)->setTextures(wheelTexture[qrand()%5], bodyTexture[qrand()%20], bodyTexture[qrand()%20], bodyTexture[qrand()%20]);
}

// ----------------------------------------------------------------------
void QGLDraw::resizeGL(int nWidth, int nHeight)
{
    WindowHeight = nHeight;
    WindowWidth = nWidth;

    GLfloat aspectRatio;
    //отслеживание изменений ширины и высоты окна
    GLfloat windowWidth,windowHeight;

    //предотвращает деление на ноль
    if(nHeight == 0)
        nHeight = 1;

    //устанавливает поле просмотра с размерами окна
    glViewport(0, 0, nWidth, nHeight);

    //обновляет систему координат
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //с помощью плоскостей отсечения устанавливает обьем отсечения
    aspectRatio = (GLfloat)nWidth / (GLfloat)nHeight;
    if (nWidth <= nHeight)
    {
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        glOrtho (-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
    }
    else
    {
        windowWidth = 100 * aspectRatio;
        windowHeight = 100;
        glOrtho (-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ----------------------------------------------------------------------
void QGLDraw::paintGL()
{
    //CalculateFrameRate(); //считает и выводит FPS

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //очистка буферов заданным цветом

    if(PrsKey[0])
        UnitBase->value(0)->moveForward();

    if(PrsKey[1])
        UnitBase->value(0)->moveBack();

    if(PrsKey[2])
        UnitBase->value(0)->rotateBody(0.05);

    if(PrsKey[3])
        UnitBase->value(0)->rotateBody(-0.05);

    if(PrsKey[4])
    {
        UnitBase->value(0)->tankShot();
    }

    //передвижение камеры исходя из координат танка
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WindowWidth / scale, 0, WindowHeight / scale, 150, -150);

    glTranslated(-UnitBase->value(0)->getxPos()+(WindowWidth / scale)/2, -UnitBase->value(0)->getyPos()+(WindowHeight / scale)/2, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //отрисовка части земли с песком
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, worldTexture[0]);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1000.0f, -1000.0f, 0.0f);
    glTexCoord2f(5.0f, 0.0f);
    glVertex3f(1000.0f, -1000.0f, 0.0f);
    glTexCoord2f(5.0f, 5.0f);
    glVertex3f(1000.0f, 1000.0f, 0.0f);
    glTexCoord2f(0.0f, 5.0f);
    glVertex3f(-1000.0f, 1000.0f, 0.0f);
    glEnd();

    //обновление физического мира с 60 кадрами в секунду
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 20.0f;
    int32 positionIterations = 20.0f;
    world->Step(timeStep, velocityIterations, positionIterations);

    //world->DrawDebugData();

    //отрисовка юнитов
    foreach(AbstractUnit * unt, UnitBase->values())
    {
        unt->drawUnit(scale);
    }

    foreach(AbstractEffect * unt, EffectBase->values())
    {
        if(!unt->draw())
        {
            int id = unt->getId();
            delete EffectBase->value(id);
            EffectBase->remove(id);
        }
    }
}
//----------------------------------------------------------------------------

void QGLDraw::CalculateFrameRate()
{
    static int framesPerSecond = 0;
    static int lastTime = 0;

    QTime time = QTime::currentTime();
    int currentTime = time.second();
    ++framesPerSecond;

    if(currentTime - lastTime > 1)
    {
        lastTime = currentTime;

        FPS=framesPerSecond;

        char ch[15];
        sprintf(ch, "FPS:%d", FPS);
        setWindowTitle(QString(ch));
        //qDebug() << "FPS:" << FPS;


        framesPerSecond = 0;
    }
}



void QGLDraw::keyPressEvent(QKeyEvent * ev)
{
    if(ev->key() == Qt::Key_Up)
    {
        PrsKey[0] = 1;
        return;
    }
    else if(ev->key() == Qt::Key_Down)
    {
        PrsKey[1] = 1;
        return;
    }
    else if(ev->key() == Qt::Key_Left)
    {
        PrsKey[2] = 1;
        return;
    }
    else if(ev->key() == Qt::Key_Right)
    {
        PrsKey[3] = 1;
        return;
    }
    else if(ev->key() == Qt::Key_Escape)
    {
        this->close();
    }
    else
    {
        add_bot();
        //qDebug() << "WindowHeight = " <<WindowHeight << " WindowWidth = " <<WindowWidth;
        return;
    }
}
//---------------------------------------------------------------------

void QGLDraw::keyReleaseEvent(QKeyEvent * ev)
{
    //освобождение клавиши что бы предотвратить ее применение в дальнейшем
    if(ev->key() == Qt::Key_Up)
    {
        PrsKey[0] = 0;
    }
    if(ev->key() == Qt::Key_Down)
    {
        PrsKey[1] = 0;
    }
    if(ev->key() == Qt::Key_Left)
    {
        PrsKey[2] = 0;
    }
    if(ev->key() == Qt::Key_Right)
    {
        PrsKey[3] = 0;
    }
}

void QGLDraw::mouseMoveEvent(QMouseEvent * ev)
{
    qreal rotate = atan2(toGl_y(ev->y()), toGl_x(ev->x())) / 0.017453293/*(3.1415926535 / 180)*/;

       UnitBase->value(0)->rotateGun(rotate);

}

void QGLDraw::mousePressEvent(QMouseEvent * ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        PrsKey[4] = 1;
    }
}

void QGLDraw::mouseReleaseEvent(QMouseEvent * ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        PrsKey[4] = 0;
    }
}

void QGLDraw::wheelEvent(QWheelEvent * ev)
{
    if(ev->delta() > 20)
    {
        if(scale < 5)
        {
            scale += 0.10;
            scale += 0.10;
        }
    }
    else
    {
        if(scale > 0.2)
        {
            scale -= 0.10;
            scale -= 0.10;
        }
    }
    qDebug()<<"Scale:"<<scale;
}

void QGLDraw::add_bot()
{
    int id=0;
    while(1)
    {
        id = qrand()%50000;
        if(!UnitBase->contains(id))
        {
            //qDebug() << "Break";
            break;
        }
    }

    int rand = qrand()%5;

    //qDebug() << "Create=" <<rand;

    switch(rand)
    {
    case 0:
    {
       AbstractUnit * reg = new TankUnit(NULL, (qrand()%500 + (0-qrand()%500)), (qrand()%500 + (0-qrand()%500)), world, id, UnitBase);
       connect(tmDrw, SIGNAL(timeout()), reg, SLOT(botGaming()));
       UnitBase->insert(id, reg);
       UnitBase->value(id)->setTextures(wheelTexture[qrand()%5], bodyTexture[qrand()%20], bodyTexture[qrand()%20], bodyTexture[qrand()%20]);
    }
    break;

    case 1:
    {
        AbstractUnit * reg = new DoubleTank(NULL,  (qrand()%500 + (0-qrand()%500)), (qrand()%500 + (0-qrand()%500)), world, id, UnitBase);
       connect(tmDrw, SIGNAL(timeout()), reg, SLOT(botGaming()));
       UnitBase->insert(id, reg);
       UnitBase->value(id)->setTextures(wheelTexture[qrand()%5], bodyTexture[qrand()%20], bodyTexture[qrand()%20], bodyTexture[qrand()%20]);
    }
    break;

    case 2:
    {
        AbstractUnit * reg = new CarUnit(NULL,  (qrand()%500 + (0-qrand()%500)), (qrand()%500 + (0-qrand()%500)), world, id, UnitBase);
       connect(tmDrw, SIGNAL(timeout()), reg, SLOT(botGaming()));
       UnitBase->insert(id, reg);
    }
    break;

    case 3:
    {
        AbstractUnit * reg = new BmpUnit(NULL,  (qrand()%500 + (0-qrand()%500)), (qrand()%500 + (0-qrand()%500)), world, id, UnitBase);
       connect(tmDrw, SIGNAL(timeout()), reg, SLOT(botGaming()));
       UnitBase->insert(id, reg);
    }
    break;

    case 4:
    {
        AbstractUnit * reg = new UfoUnit(NULL,  (qrand()%500 + (0-qrand()%500)), (qrand()%500 + (0-qrand()%500)), world, id, UnitBase);
       connect(tmDrw, SIGNAL(timeout()), reg, SLOT(botGaming()));
       UnitBase->insert(id, reg);
    }
    break;
    }
}



QMap<int,AbstractUnit *> * QGLDraw::getUnitBase()
{
    return UnitBase;
}

b2World * QGLDraw::getWorld()
{
    return world;
}
