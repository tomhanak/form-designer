include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(gui::main_window)

HEADERS += ../Editor.hpp
HEADERS += ../GraphicsViewMagnifier.hpp
HEADERS += ../MainWindow.hpp
HEADERS += ./AboutDialog.hpp
HEADERS += ./MDI.hpp
HEADERS += ./SettingsPage.hpp
PRIV_HEADERS += Editor_p.hpp
PRIV_HEADERS += GraphicsViewMagnifier_p.hpp
PRIV_HEADERS += MainWindow_p.hpp
SOURCES += AboutDialog.cpp
SOURCES += Editor.cpp
SOURCES += GraphicsViewMagnifier.cpp
SOURCES += MainWindow.cpp
SOURCES += MDI.cpp
SOURCES += SettingsPage.cpp
RESOURCES += ./gui.main_window.qrc
FORMS += ./AboutDialog.ui
FORMS += ./MainWindow.ui
FORMS += ./SettingsPage.ui
LIBS	+= $$require_sfd_component(./../../.., core::project)
