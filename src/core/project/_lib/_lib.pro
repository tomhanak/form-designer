include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(core::project)

HEADERS += ../Document.hpp
HEADERS += ../Project.hpp
HEADERS += ../Region.hpp
HEADERS += ../Resource.hpp
HEADERS += ../TerminalInfo.hpp
PRIV_HEADERS += Document_p.hpp
PRIV_HEADERS += Project_p.hpp
PRIV_HEADERS += Region_p.hpp
PRIV_HEADERS += Resource_p.hpp
PRIV_HEADERS += TerminalInfo_p.hpp
SOURCES += Document.cpp
SOURCES += Project.cpp
SOURCES += Region.cpp
SOURCES += Resource.cpp
SOURCES += TerminalInfo.cpp
