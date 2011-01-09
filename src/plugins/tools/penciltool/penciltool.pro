# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/tools/penciltool
# Target is a library:  

INSTALLS += target 
target.path = /plugins/

HEADERS += brush.h \
           exactnessconfigurator.h
SOURCES += brush.cpp \
           exactnessconfigurator.cpp

CONFIG += plugin warn_on
TEMPLATE = lib 
TARGET = penciltool

FRAMEWORK_DIR = "../../../framework"
include($$FRAMEWORK_DIR/framework.pri)
include(../tools_config.pri)

include(../../../../tupiglobal.pri)
