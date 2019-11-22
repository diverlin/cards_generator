QT += widgets

HEADERS     = mainwindow.hpp \
              dragwidget.hpp \
              draglistwidget.hpp \
              card.hpp \
              colors.hpp \
              utils.hpp \

RESOURCES   =

SOURCES     = mainwindow.cpp \
              dragwidget.cpp \
              draglistwidget.cpp \
              utils.cpp \
              card.cpp \
              colors.cpp \
              main.cpp

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/draganddrop/draggableicons
#INSTALLS += target
