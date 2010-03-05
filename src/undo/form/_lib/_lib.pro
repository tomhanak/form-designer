include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(undo::form)

HEADERS += ../AddControl.hpp
HEADERS += ../RemoveControl.hpp
HEADERS += ../RemoveControls.hpp
HEADERS += ../RenameControl.hpp
PRIV_HEADERS += AddControl_p.hpp
PRIV_HEADERS += RemoveControl_p.hpp
PRIV_HEADERS += RemoveControls_p.hpp
PRIV_HEADERS += RenameControl_p.hpp
SOURCES += AddControl.cpp
SOURCES += RemoveControl.cpp
SOURCES += RemoveControls.cpp
SOURCES += RenameControl.cpp
