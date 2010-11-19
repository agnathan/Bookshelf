HEADERS       = connection.h \
    MainWindow.h \
    accountinfodialog.h
SOURCES       = tablemodel.cpp \
    MainWindow.cpp \
    accountinfodialog.cpp
QT           += sql
QT           += network xml
# install
target.path = $$[QT_INSTALL_EXAMPLES]/sql/tablemodel
sources.files = $$SOURCES *.h $$RESOURCES $$FORMS tablemodel.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/sql/tablemodel
INSTALLS += target sources

FORMS += \
    accountinfodialog.ui
