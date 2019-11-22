QT += widgets

HEADERS     = mainwindow.hpp \
              cardboard.hpp \
              imagespoolwidget.hpp \
              imagewidget.hpp \
              colors.hpp \
              overlaylabel.hpp \
              utils.hpp \

RESOURCES   = \
    cards_generator.qrc

SOURCES     = mainwindow.cpp \
              cardboard.cpp \
              imagespoolwidget.cpp \
              utils.cpp \
              imagewidget.cpp \
              colors.cpp \
              overlaylabel.cpp \
              main.cpp



win32|win64 {
    CONFIG(release, debug|release) {
        DEFINES += QT_NO_DEBUG_OUTPUT
        CONFIG += static # link qt libs statically
        LIBS += -static # link rest libs statically
        #QTPLUGIN += qwindows
    }
}
