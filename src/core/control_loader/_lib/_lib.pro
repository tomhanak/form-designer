include(./../../../common.pri)

TEMPLATE	= lib
TARGET		= $$sfd_component_library_target(core::control_loader)

HEADERS += ../ControlPlugin.hpp
HEADERS += ../ControlPluginLoader.hpp
PRIV_HEADERS += ControlPluginLoader_p.hpp
PRIV_HEADERS += ControlPlugin_p.hpp
SOURCES += ControlPlugin.cpp
SOURCES += ControlPluginLoader.cpp
