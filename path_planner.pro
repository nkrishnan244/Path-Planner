QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#CONFIG += console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    astar.cpp \
    dijkstra.cpp \
    dstar.cpp \
    helper_functions.cpp \
    main.cpp \
    mainwindow.cpp \
    node.cpp \
    occupancy_grid.cpp \
    planner.cpp \
#    plotgrid.cpp \
#    probabilisticroadmap.cpp \
    point.cpp \
    qcustomplot.cpp \
#    rrt.cpp
 \#    plotter.cpp
    qlearning.cpp \
    rrt.cpp

HEADERS += \
    astar.h \
    dijkstra.h \
    dstar.h \
    mainwindow.h \
    matplotlibcpp.h \
    node.h \
    occupancy_grid.h \
    planner.h \
#    plotgrid.h \
#    probabilisticroadmap.h \
    point.h \
    qcustomplot.h \
#    rrt.h
 \#    plotter.h
    qlearning.h \
    rrt.h

#INCLUDEPATH += \
#    /usr/include/python2.7 \
#    /usr/local/lib/python2.7/dist-packages/numpy/core/include

#LIBS += \
#    -L/python2.7

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
