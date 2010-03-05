include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(gui::property_editor)

HEADERS += ../PropertyEditor.hpp
HEADERS += ./ResetButton.hpp
HEADERS += ./TreePropertyWidget.hpp
PRIV_HEADERS += PropertyEditor_p.hpp
SOURCES += PropertyEditor.cpp
SOURCES += ResetButton.cpp
SOURCES += TreePropertyWidget.cpp
LIBS	+= $$require_sfd_component(./../../.., undo::property_editor)

include(./../../../../ext/qtpropertybrowser-2.5_1-opensource/src/qtpropertybrowser.pri)
