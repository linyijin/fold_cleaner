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

INCLUDEPATH+=D:\liinyijin\astar\include\

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
    src/path_plan/planner.cpp \
    src/path_plan/astar.cpp \
    src/base_move/move.cpp \
    src/path_plan/search.cpp \
    src/base_move/pose2d.cpp \
    src/state/state.cpp \
    src/hsm.cpp \
    src/state/state_init.cpp \
    src/state/state_move.cpp \
    src/schedule_fold.cpp \
    src/state/state_turn.cpp \
    src/state/state_followy.cpp \
    src/base_move/controller.cpp \
    src/state/state_nav.cpp \
    src/state/state_followwall.cpp

HEADERS += \
    include/dstar.h \
        include/mainwindow.h \
    include/astar.h \
    include/planner.h \
    include/move.h \
    include/search.h \
    include/pose2d.h \
    include/state.h \
    include/hsm.h \
    include/state_init.h \
    include/state_move.h \
    include/schedule_fold.h \
    include/state_turn.h \
    include/state_followy.h \
    include/controller.h \
    include/state_nav.h \
    include/state_followwall.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
