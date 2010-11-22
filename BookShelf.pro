#-------------------------------------------------
#
# Project created by QtCreator 2010-11-19T21:39:20
#
#-------------------------------------------------

QT       += core gui sql network xml

TARGET = BookShelf
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    accountinfodialog.cpp \
    bookdetailsview.cpp

HEADERS  += mainwindow.h \
    accountinfodialog.h \
    connection.h \
    bookdetailsview.h

FORMS    +=  accountinfodialog.ui
