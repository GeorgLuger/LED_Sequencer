QT       += core gui
QT += sql
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    auswahleffektewindow.cpp \
    auswahlsequenzwindow.cpp \
    ccolour.cpp \
    ceffekte.cpp \
    csequenzen.cpp \
    deleteallsequenzen.cpp \
    detailsequenzenwindow.cpp \
    exportsqlwindow.cpp \
    importsqlwindow.cpp \
    listeeffektewindow.cpp \
    listesequenzenwindow.cpp \
    loeschesequenzwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    sendeeffektwindow.cpp \
    sendesequenzwindow.cpp \
    timerwindow.cpp

HEADERS += \
    auswahleffektewindow.h \
    auswahlsequenzwindow.h \
    ccolour.h \
    ceffekte.h \
    csequenzen.h \
    deleteallsequenzen.h \
    detailsequenzenwindow.h \
    exportsqlwindow.h \
    importsqlwindow.h \
    listeeffektewindow.h \
    listesequenzenwindow.h \
    loeschesequenzwindow.h \
    mainwindow.h \
    sendeeffektwindow.h \
    sendesequenzwindow.h \
    sqldatabase.h \
    timerwindow.h

FORMS += \
    auswahleffektewindow.ui \
    auswahlsequenzwindow.ui \
    deleteallsequenzen.ui \
    detailsequenzenwindow.ui \
    exportsqlwindow.ui \
    importsqlwindow.ui \
    listeeffektewindow.ui \
    listesequenzenwindow.ui \
    loeschesequenzwindow.ui \
    mainwindow.ui \
    sendeeffektwindow.ui \
    sendesequenzwindow.ui \
    timerwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
