include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(undo::transl_table)

HEADERS += ../AddId.hpp
HEADERS += ../AddLanguage.hpp
HEADERS += ../CloneId.hpp
HEADERS += ../EditText.hpp
HEADERS += ../RemoveId.hpp
HEADERS += ../RemoveLanguage.hpp
HEADERS += ../RenameId.hpp
HEADERS += ../RenameLanguage.hpp
PRIV_HEADERS += AddId_p.hpp
PRIV_HEADERS += AddLanguage_p.hpp
PRIV_HEADERS += CloneId_p.hpp
PRIV_HEADERS += EditText_p.hpp
PRIV_HEADERS += RemoveId_p.hpp
PRIV_HEADERS += RemoveLanguage_p.hpp
PRIV_HEADERS += RenameId_p.hpp
PRIV_HEADERS += RenameLanguage_p.hpp
SOURCES += AddId.cpp
SOURCES += AddLanguage.cpp
SOURCES += CloneId.cpp
SOURCES += EditText.cpp
SOURCES += RemoveId.cpp
SOURCES += RemoveLanguage.cpp
SOURCES += RenameId.cpp
SOURCES += RenameLanguage.cpp
LIBS	+= $$require_sfd_component(./../../.., core::transl_table)
