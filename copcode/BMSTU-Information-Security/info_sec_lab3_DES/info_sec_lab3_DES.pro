TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        des.cpp \
        main.cpp \
        utils.cpp

HEADERS += \
    DES_params.h \
    des.h \
    initkey.hpp \
    mybitset.hpp \
    utils.h
