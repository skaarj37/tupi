# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/tools/geometrictool
# Target is a library:  

INSTALLS += target 
target.path = /plugins/

HEADERS += geometrictool.h \
           infopanel.h 
SOURCES += geometrictool.cpp \
           infopanel.cpp

CONFIG += plugin
TEMPLATE = lib 
TARGET = tupigeometrictool

FRAMEWORK_DIR = "../../../framework"
include($$FRAMEWORK_DIR/framework.pri)
include(../tools_config.pri)

include(../../../../tupiglobal.pri)
