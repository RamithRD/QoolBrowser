#-------------------------------------------------
#
# Project created by QtCreator 2016-11-29T19:02:03
#
#-------------------------------------------------

QT       += core gui webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QoolBrowser
TEMPLATE = app


SOURCES += main.cpp\
        browsermainwindow.cpp \
    qoollineedit.cpp \
    addbookmarkdialog.cpp \
    historywindow.cpp \
    bookmarks.cpp \
    qoolutils.cpp

HEADERS  += browsermainwindow.h \
    qoollineedit.h \
    addbookmarkdialog.h \
    historywindow.h \
    bookmarks.h \
    qoolutils.h

FORMS    += browsermainwindow.ui \
    addbookmarkdialog.ui \
    historywindow.ui \
    bookmarks.ui

RESOURCES += \
    browserassets.qrc
