include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(mva::control_loader)

HEADERS += ../ControlBoxModel.hpp
PRIV_HEADERS += ControlBoxModel_p.hpp
SOURCES += ControlBoxModel.cpp
