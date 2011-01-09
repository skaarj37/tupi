# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/framework/tsound
# Target is a library: tupifwsound  

INSTALLS += target headers
target.path = /lib/ 
headers.path = /include/tupisound
headers.files += *.h

HEADERS += kaudioengineiface.h \
           kaudioplayer.h
SOURCES += kaudioplayer.cpp

CONFIG += release warn_on dll
TEMPLATE = lib 
TARGET = tupifwsound

INCLUDEPATH += ../tcore ../
LIBS += -L../tcore -ltupifwcore

linux-g{
    TARGETDEPS += ../tcore/libtupifwcore.so
}

include(../tupconfig.pri)
