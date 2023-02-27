QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    all_data.cpp \
    filedb.cpp \
    formhier.cpp \
    forminfo.cpp \
    formlayer.cpp \
    formmap.cpp \
    formtop.cpp \
    main.cpp \
    mainwindow.cpp \
    trianglerenderer.cpp

HEADERS += \
    all_data.h \
    filedb.h \
    formhier.h \
    forminfo.h \
    formlayer.h \
    formmap.h \
    formtop.h \
    mainwindow.h \
    mainwindow.h \
    trianglerenderer.h

FORMS += \
    formhier.ui \
    forminfo.ui \
    formlayer.ui \
    formmap.ui \
    formtop.ui \
    mainwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
