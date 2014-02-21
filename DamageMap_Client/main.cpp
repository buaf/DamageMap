//#include "glpaintscene.h"

#include <QApplication>

#include <Box2D/Box2D.h>


int main(int argc, char** argv)
{
    QApplication app(argc, argv);


    b2Vec2 * gravity = new b2Vec2(0.0f, 0.0f);
    b2World * world = new b2World(*gravity, 0);

    //QGLDraw * gameworld = new QGLDraw(0);
    //gameworld->show();

    return app.exec();
}
