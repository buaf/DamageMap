#ifndef OGLDRAW_H
#define OGLDRAW_H

#include <QGLWidget> //для OpenGL
#include <QTimer>    //таймер для постоянной роботы приложения
#include <QObject>   //кэп
#include <QKeyEvent> //событие нажатие клавиш
#include <QDebug>    //для дебагинга
#include <QWidget>   //кэп
#include <QMap>      //в QMap будет хранится множество обьектов

#include "Units/tankunit.h" //будет содержать информацию о местоположении танка , и отслеживать столкновения
#include "Bullets/unitbullet.h" //будет содержать информацию про пулю выпущеную из танка
#include "Effects/abstracteffect.h"
#include "Effects/explode1.h"

#include "Box2D/Box2D.h"
#include "b2debugdraw.h"

class AbstractEffect;

struct gameInfo
{
    gameInfo()
    {
        serverPort = 0, serverSize = 0, clientAdress = " ", clientPort = 0,
        botCount = 1, width = 800, height = 800, isFullScreen = 0, gameMode = 0, playerName = "";
    }

    //настройки сервера
    int serverPort;
    int serverSize;

    //настройки клиента
    QString clientAdress;
    int clientPort;

    //общие настройки
    int botCount, width, height;
    bool isFullScreen;
    QString playerName;

    int gameMode; //0-одиночная игра, 1-Создать сетевую игру, 2-подключиться к сетевой игре
};


class QGLDraw : public QGLWidget
{
    Q_OBJECT

private:
    int FPS; //кагбы кэп
    bool isClientMode; //равно 1 если в режиме клиента

    //физический мир
    b2Vec2 * gravity;
    b2World * world;
    DebugDraw * debugDraw; //дебаг отрисовка

    GLuint wheelTexture[10]; //текстуры колес
    GLuint bodyTexture[20];  //текстуры тела
    GLuint worldTexture[5]; //текстуры мира


    QTimer * tmDrw; //таймер для общего обновления всего

    //тут будет хранится информация обо всех танках (кооррдинаты , углы разворота)
    QMap<int, AbstractUnit *> * UnitBase;
    QMap<int, AbstractEffect *> * EffectBase;

    int WindowHeight; //высота окна
    int WindowWidth; //ширина окна
    float scale; //увеличение и уменьшение

    int startBotCount;

    //нажатые клавишы
    bool PrsKey[20]; //0 - кнопка вверх , 1 - кнопка вниз , 2 - кнопка лево , 3 - кнопка право , 4- кнопка стрельбы

    //преобразование координат окна в координаты OpenGL для мышки
    int toGl_x(int mouse_x);
    int toGl_y(int mouse_y);

public:
    QGLDraw(QWidget * pwgt = 0, gameInfo * info = 0);

    void loadTextures(); //загрузить текстуры

    //OpenGL
    virtual void initializeGL();
    virtual void resizeGL(int nWidth, int nHeight);
    virtual void paintGL();
    void CalculateFrameRate(); //подсчет FPS

    //События для управления
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
