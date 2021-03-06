# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/export/smilexportplugin
# Target is a library:  

INSTALLS += target 
target.path = /plugins/ 

macx {
    CONFIG += plugin

    INSTALLS += otool
    otool.path = /lib/
    otool.extra  += ../../../../tools/update_dylib_path.rb $(INSTALL_ROOT)/lib/$(TARGET) '^/lib' '$(INSTALL_ROOT)/lib'
}

HEADERS += smilexportplugin.h
SOURCES += smilexportplugin.cpp

CONFIG += plugin warn_on
TEMPLATE = lib 
TARGET = tupismilexportplugin

FRAMEWORK_DIR = "../../../framework"
include($$FRAMEWORK_DIR/framework.pri)

include(../export_config.pri)
include(../../../../tupiglobal.pri)
