include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(mva::transl_table)

HEADERS += ../TranslTableModel.hpp
PRIV_HEADERS += TranslTableModel_p.hpp
SOURCES += TranslTableModel.cpp
LIBS	+= $$require_sfd_component(./../../.., core::transl_table)
LIBS	+= $$require_sfd_component(./../../.., undo::transl_table)
