include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(undo::project)

HEADERS += ../AddForm.hpp
HEADERS += ../AddRegion.hpp
HEADERS += ../AddResource.hpp
HEADERS += ../AddTranslTable.hpp
HEADERS += ../RemoveForm.hpp
HEADERS += ../RemoveRegion.hpp
HEADERS += ../RemoveResource.hpp
HEADERS += ../RemoveTranslTable.hpp
PRIV_HEADERS += AddForm_p.hpp
PRIV_HEADERS += AddRegion_p.hpp
PRIV_HEADERS += AddResource_p.hpp
PRIV_HEADERS += AddTranslTable_p.hpp
PRIV_HEADERS += RemoveForm_p.hpp
PRIV_HEADERS += RemoveRegion_p.hpp
PRIV_HEADERS += RemoveResource_p.hpp
PRIV_HEADERS += RemoveTranslTable_p.hpp
SOURCES += AddForm.cpp
SOURCES += AddRegion.cpp
SOURCES += AddResource.cpp
SOURCES += AddTranslTable.cpp
SOURCES += RemoveForm.cpp
SOURCES += RemoveRegion.cpp
SOURCES += RemoveResource.cpp
SOURCES += RemoveTranslTable.cpp
