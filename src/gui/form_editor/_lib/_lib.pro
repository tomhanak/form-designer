include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(gui::form_editor)

HEADERS += ../FormEditor.hpp
HEADERS += ./GeometryEditor.hpp
HEADERS += ./GridSettingsPage.hpp
PRIV_HEADERS += FormEditor_p.hpp
SOURCES += FormEditor.cpp
SOURCES += GeometryEditor.cpp
SOURCES += GridSettingsPage.cpp
FORMS += ./GridSettingsPage.ui
LIBS	+= $$require_sfd_component(./../../.., core::control_loader)
LIBS	+= $$require_sfd_component(./../../.., gui::main_window)
LIBS	+= $$require_sfd_component(./../../.., undo::property_editor)
