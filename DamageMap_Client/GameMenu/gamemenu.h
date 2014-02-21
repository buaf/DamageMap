#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "glpaintscene.h"

#include <QDialog>
#include <QWidget>
#include <QString>



class QGameMenu : public QDialog
{
    Q_OBJECT
public:
    QGameMenu(QDialog * parent = 0);
    ~QGameMenu();
    gameInfo * getInfo();

private:
   gameInfo * info;

private slots:
   void Settings();
};

#endif // GAMEMENU_H
