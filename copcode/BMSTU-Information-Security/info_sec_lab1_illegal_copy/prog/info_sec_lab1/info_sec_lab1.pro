TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        device_hdd_serial.cpp \
        installer.cpp \
        main.cpp

HEADERS += \
    device_hdd_serial.h \
    installer.h
