# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/tools/schemetool
# Target is a library:  

INSTALLS += target 
target.path = /plugins/

HEADERS += schemetool.h \
           configurator.h
SOURCES += schemetool.cpp \
           configurator.cpp

CONFIG += plugin warn_on
TEMPLATE = lib 
TARGET = tupischemetool

FRAMEWORK_DIR = "../../../framework"
include($$FRAMEWORK_DIR/framework.pri)
include(../tools_config.pri)

include(../../../../tupiglobal.pri)
