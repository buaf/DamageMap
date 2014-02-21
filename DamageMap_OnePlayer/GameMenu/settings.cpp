#include "settings.h"
#include "gamemenu.h"

#include <QDialog>
#include <QButtonGroup>
#include <QPalette>

QSettings::QSettings(QDialog *parent) : QDialog(parent)
{
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::black);
    setPalette(p);
}

QSettings::~QSettings()
{

}

gameInfo * QSettings::getInfo()
{
    return info;
}


