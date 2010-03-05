include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(core::transl_table)

HEADERS += ../UnifiedTranslTable.hpp
PRIV_HEADERS += UnifiedTranslTable_p.hpp
SOURCES += UnifiedTranslTable.cpp
LIBS	+= $$require_sfd_component(./../../.., core::project)
