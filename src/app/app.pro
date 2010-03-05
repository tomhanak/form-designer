ROOT_REL_PATH = ../
include($$ROOT_REL_PATH/common.pri)

TEMPLATE = app
TARGET   = sfd
HEADERS  = msghndl.hpp
SOURCES  = main.cpp msghndl.cpp

LIBS += $$require_sfd_component($$ROOT_REL_PATH, core::transl_table)
LIBS += $$require_sfd_component($$ROOT_REL_PATH, gui::transl_table)
LIBS += $$require_sfd_component($$ROOT_REL_PATH, core::form)
LIBS += $$require_sfd_component($$ROOT_REL_PATH, core::project)
LIBS += $$require_sfd_component($$ROOT_REL_PATH, core::control_loader)
LIBS += $$require_sfd_component($$ROOT_REL_PATH, gui::form_editor)
LIBS += $$require_sfd_component($$ROOT_REL_PATH, gui::main_window)
LIBS += $$require_sfd_component($$ROOT_REL_PATH, gui::property_editor)
