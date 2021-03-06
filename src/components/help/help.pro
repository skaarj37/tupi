# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/help
# Target is a library:  

INSTALLS += help 
help.files += help 
help.path = /data/ 

INSTALLS += target
target.path = /lib/

macx {
    CONFIG += staticlib warn_on
}

HEADERS += tuphelpwidget.h \
           tuphelpbrowser.h 
SOURCES += tuphelpwidget.cpp \
           tuphelpbrowser.cpp 
*:!macx{
    CONFIG += dll warn_on
}

TEMPLATE = lib
TARGET = tupihelp

FRAMEWORK_DIR = "../../framework"
include($$FRAMEWORK_DIR/framework.pri)
include(../components_config.pri)
