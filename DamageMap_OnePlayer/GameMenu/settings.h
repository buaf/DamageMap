#ifndef SETTINGS_H
#define SETTINGS_H

#include "glpaintscene.h"

#include <QDialog>
#include <QWidget>
#include <QString>



class QSettings : public QDialog
{
    Q_OBJECT
public:
    QSettings(QDialog * parent = 0);
    ~QSettings();
    gameInfo * getInfo();

private:
    gameInfo * info;

};

#endif // SETTINGS_H
