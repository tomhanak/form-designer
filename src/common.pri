INCLUDEPATH	+=$$PWD
DEPENDPATH	+=$$PWD
ROOT_PATH		= $$PWD

defineReplace(sfd_component_library) {
	return(sfd_$$replace(1, '::', '_'))
}

defineReplace(sfd_component_library_path) {
	return(${PWD}/$$1/$$replace(2, '::', '/')/_lib/)
}

defineReplace(sfd_component_library_target) {
	return($$qtLibraryTarget($$sfd_component_library($$1)))
}

defineReplace(require_sfd_component) {
	return(-L$$sfd_component_library_path($$1, $$2) 			-l$$sfd_component_library($$2) 			-Xlinker -R$$sfd_component_library_path($$1, $$2))
}

# Prevent MOC to process *_p.hpp headers automatically
# Inspired by QMake's internals...
moc_priv_header.CONFIG = no_link moc_verify
moc_priv_header.dependency_type = TYPE_C
moc_priv_header.commands = ${QMAKE_FUNC_mocCmd}
moc_priv_header.output = ${QMAKE_FILE_BASE}.moc
moc_priv_header.input = PRIV_HEADERS
moc_priv_header.name = MOC ${QMAKE_FILE_IN}
silent:moc_priv_header.commands = @echo moc ${QMAKE_FILE_IN} && $$moc_priv_header.commands
QMAKE_EXTRA_COMPILERS += moc_priv_header
INCREDIBUILD_XGE += moc_priv_header
build_pass|isEmpty(BUILDS):priv_headers_mocables.depends = compiler_moc_priv_header_make_all
else:priv_headers_mocables.CONFIG += recursive
QMAKE_EXTRA_TARGETS += priv_headers_mocables

QMAKE_CXXFLAGS	+= -include common.hpp


CONFIG += debug

QMAKE_CXXFLAGS  += -Wall
QMAKE_CXXFLAGS  += -Wextra
QMAKE_CXXFLAGS  += -Werror
QMAKE_CXXFLAGS  += -Wno-error=deprecated-declarations

QMAKE_EXT_H = .hpp

