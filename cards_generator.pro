QT += widgets

HEADERS     = mainwindow.hpp \
              dragwidget.hpp \
              draglistwidget.hpp
RESOURCES   =
SOURCES     = mainwindow.cpp \
              dragwidget.cpp \
              draglistwidget.cpp \
              main.cpp

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/draganddrop/draggableicons
#INSTALLS += target
