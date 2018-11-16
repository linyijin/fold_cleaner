#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T11:36:50
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = astar
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    planner.cpp \
    astar.cpp \
    move.cpp \
    search.cpp \
    pose2d.cpp \
    state.cpp \
    hsm.cpp \
    state_init.cpp \
    state_move.cpp \
    schedule_fold.cpp \
    state_turn.cpp \
    state_followy.cpp \
    controller.cpp \
    state_nav.cpp \
    state_followwall.cpp

HEADERS += \
        mainwindow.h \
    astar.h \
    planner.h \
    move.h \
    search.h \
    pose2d.h \
    state.h \
    hsm.h \
    state_init.h \
    state_move.h \
    schedule_fold.h \
    state_turn.h \
    state_followy.h \
    controller.h \
    state_nav.h \
    state_followwall.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
