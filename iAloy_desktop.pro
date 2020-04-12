#-------------------------------------------------
#
# Project created by QtCreator 2020-02-12T22:45:38
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iAloy_desktop
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
	src/main.cpp \
	src/mainwindow.cpp \
	src/connect_init.cpp \
	src/hardware.cpp \
	src/dashboard.cpp \
	src/settings.cpp \
	src/dev_controller_api.cpp \
	src/update_manager.cpp \
	src/update_manager_thread.cpp \


HEADERS += \
	src/mainwindow.h \
	src/connect_init.h \
	src/hardware.h \
	src/dashboard.h \
	src/settings.h \
	src/dev_controller_api.h \
	src/update_manager.h \
	src/update_manager_thread.h \


FORMS += \
	resource/ui/mainwindow.ui \
	resource/ui/dashboard.ui \
	resource/ui/settings.ui \
	resource/ui/update_manager.ui \


OTHER_FILES += \
	src/Makefile \
	resource/Makefile \
	resource/scripts/iAloy-interface-up.sh \
	resource/scripts/update_check.sh

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
