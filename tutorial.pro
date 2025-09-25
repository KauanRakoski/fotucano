QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    HistogramVisualizer.cpp \
    ImageProcessor.cpp \
    Sidebar.cpp \
    histogramscontainer.cpp \
    kerneleditor.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    HistogramVisualizer.h \
    ImageProcessor.h \
    SideBar.h \
    histogramscontainer.h \
    kerneleditor.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
