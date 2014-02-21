#-------------------------------------------------
#
# Project created by QtCreator 2013-03-26T21:07:44
#
#-------------------------------------------------

QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DamageMap_Client
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp #\
    #glpaintscene.cpp \
    #b2debugdraw.cpp \
    #b2mycontactfilter.cpp \
    #bodyinfo.cpp \
    #Units/abstractunit.cpp \
    #Bullets/abstractbullet.cpp \
    #Units/tankunit.cpp \
    #Bullets/unitbullet.cpp \
    #Units/doubletank.cpp \
    #Effects/abstracteffect.cpp \
    #Effects/explode1.cpp \
    #Units/bmpunit.cpp \
    #Units/ufounit.cpp \
    #Units/carunit.cpp

#HEADERS  += \
    #glpaintscene.h \
    #b2debugdraw.h \
    #b2mycontactfilter.h \
    #bodyinfo.h \
    #Units/abstractunit.h \
    #Bullets/abstractbullet.h \
    #Units/tankunit.h \
    #Bullets/unitbullet.h \
    #Units/doubletank.h \
    #Effects/abstracteffect.h \
    #Effects/explode1.h \
    #Units/bmpunit.h \
    #Units/ufounit.h \
    #Units/carunit.h


#BOX_2D
LIBS += /home/schrader/Box2D_v2.1.2/Box2D/Build/Box2D/libBox2D.a
INCLUDEPATH += /home/schrader/Box2D_v2.1.2/Box2D

