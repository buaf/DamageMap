#include "glpaintscene.h"
#include "GameMenu/gamemenu.h"

#include <QApplication>
//#include <GL/freeglut.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    //glutInit(&argc, argv);

    QGameMenu * menu =  new QGameMenu();
    menu->setWindowState(menu->windowState() | Qt::WindowMaximized);

    if(menu->exec() == QDialog::Accepted)
    {
        gameInfo * settins = new gameInfo();
        QGLDraw * gameworld = new QGLDraw(0, settins);
        gameworld->resize(settins->width,settins->height);
        gameworld->showFullScreen();
    }

    return app.exec();
}
