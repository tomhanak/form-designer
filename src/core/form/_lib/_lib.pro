include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(core::form)

HEADERS += ../Form.hpp
PRIV_HEADERS += Form_p.hpp
SOURCES += Form.cpp
LIBS	+= $$require_sfd_component(./../../.., core::project)
