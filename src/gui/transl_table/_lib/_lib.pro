include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(gui::transl_table)

HEADERS += ../TranslTableEditor.hpp
HEADERS += ../TranslTableView.hpp
HEADERS += ./LanguageNameDialog.hpp
HEADERS += ./TranslIdNameDialog.hpp
PRIV_HEADERS += TranslTableEditor_p.hpp
PRIV_HEADERS += TranslTableView_p.hpp
SOURCES += LanguageNameDialog.cpp
SOURCES += TranslIdNameDialog.cpp
SOURCES += TranslTableEditor.cpp
SOURCES += TranslTableView.cpp
RESOURCES += ./icons/gui.transl_table.icons.qrc
FORMS += ./LanguageNameDialog.ui
FORMS += ./TranslIdNameDialog.ui
LIBS	+= $$require_sfd_component(./../../.., core::transl_table)
LIBS	+= $$require_sfd_component(./../../.., mva::transl_table)
LIBS	+= $$require_sfd_component(./../../.., undo::transl_table)
LIBS	+= $$require_sfd_component(./../../.., gui::main_window)
