#include "gamemenu.h"
#include "settings.h"

#include <QDialog>
#include <QButtonGroup>
#include <QPalette>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QLabel>

QGameMenu::QGameMenu(QDialog *parent) : QDialog(parent)
{
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::black);
    setPalette(p);

    QPushButton * start = new QPushButton("Start game");
    start->setMaximumWidth(550);
    start->setMinimumHeight(40);

    QPushButton * settings = new QPushButton("Settings");
    settings->setMaximumWidth(550);
    settings->setMinimumHeight(40);

    QPushButton * exit = new QPushButton("Exit");
    exit->setMaximumWidth(550);
    exit->setMinimumHeight(40);

    connect(start, SIGNAL(clicked()), SLOT(accept()));
    connect(settings, SIGNAL(clicked()), SLOT(Settings()));
    connect(exit, SIGNAL(clicked()), SLOT(reject()));

    QPixmap pix("Texture/logo.png");
    QLabel * img = new QLabel();
    img->setPixmap(pix);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(img);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(start);
    layout->addWidget(settings);
    layout->addWidget(exit);

    setLayout(layout);
}

QGameMenu::~QGameMenu()
{

}

gameInfo * QGameMenu::getInfo()
{
    return info;
}



void QGameMenu::Settings()
{
    QSettings * settings =  new QSettings();
    settings->setWindowState(settings->windowState() | Qt::WindowMaximized);
    if(settings->exec() == QDialog::Accepted)
    {

    }
}


