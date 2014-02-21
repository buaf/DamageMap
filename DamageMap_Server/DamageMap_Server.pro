#QT       += core

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DamageMap_Server
TEMPLATE = app

CONFIG += c++11

#project files---------------------------------------------------
SOURCES += \
    main.cpp \
    Server/server.cpp \
    Config/config.cpp \
    World/world.cpp \
    World/Unit/abstractunit.cpp \
    World/Bullet/abstractbullet.cpp \
    World/Unit/standarttank.cpp


HEADERS  += \
    Server/server.h \
    Config/config.h \
    World/world.h \
    World/Unit/abstractunit.h \
    World/Bullet/abstractbullet.h \
    World/Unit/standarttank.h

#BOOST
LIBS += -lboost_system
LIBS += -lpthread
LIBS += -lboost_thread

#BOX_2D
LIBS += /home/schrader/Box2D_v2.1.2/Box2D/Build/Box2D/libBox2D.a
INCLUDEPATH += /home/schrader/Box2D_v2.1.2/Box2D

