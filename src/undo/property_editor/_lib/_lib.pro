include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(undo::property_editor)

HEADERS += ../EditProperty.hpp
PRIV_HEADERS += EditProperty_p.hpp
SOURCES += EditProperty.cpp
