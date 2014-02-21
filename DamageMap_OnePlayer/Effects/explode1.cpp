#include "explode1.h"

#include <math.h>

#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QListIterator>
#include <QTime>

#include <QGLWidget>
#include <QtOpenGL>
//#include <GL/freeglut.h>

Explode1::Explode1(QObject * parent, int xPos, int yPos, int size, int effectId) :
    AbstractEffect(parent)
{
    this->xPos = xPos; this->yPos = yPos; this->size = size; this->effectId = effectId;
    drawCount = 0;
    xTexPos1 = 1.0, yTexPos1 = 1.0, xTexPos2 = 0.75, yTexPos2 = 0.75;

}

Explode1::~Explode1()
{

}

bool Explode1::draw()
{    
    if(drawCount == 4 || drawCount == 8 || drawCount == 12)
    {
        xTexPos1 = 1;
        xTexPos2 = 0.75;
        yTexPos1 -= 0.25;
        yTexPos2 -= 0.25;
    }
    else if (drawCount >= 16)
    {
        return 0;
    }

    xTexPos1 -= 0.25;
    xTexPos2 -= 0.25;

    glLoadIdentity();
    glTranslatef(xPos, yPos, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textudeId);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
       glTexCoord2f(xTexPos1, yTexPos1);
       glVertex3f(-10.0f*size, 10.0f*size, 0.0f);
       glTexCoord2f(xTexPos2, yTexPos1);
       glVertex3f( 10.0f*size, 10.0f*size, 0.0f);
       glTexCoord2f(xTexPos2, yTexPos2);
       glVertex3f( 10.0f*size, -10.0f*size, 0.0f);
       glTexCoord2f(xTexPos1, yTexPos2);
       glVertex3f(-10.0f*size, -10.0f*size, 0.0f);
    glEnd();

    drawCount++;
    return 1;
}

int Explode1::getId()
{
    return effectId;
}
