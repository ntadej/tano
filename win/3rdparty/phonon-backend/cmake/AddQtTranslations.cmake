# - add_qt_translations(file1 ... fileN)
# Adds source code files (*.cpp, *.c...) to be translated.
# Creates global variable named SRCS_TRANSLATIONS
# - clear_qt_translations()
# Clears global variable named SRCS_TRANSLATIONS
#
# Example:
# clear_qt_translations()
# set(myapp_SRCS
#     toto.cpp
#     tata.cpp
# )
# add_qt_translations(${myapp_SRCS})
#
# set(TS_FILES
# 	myapp_fr.ts
# 	myapp_de.ts
# 	myapp_es.ts
# )
# set(QM_FILES "")
# qt4_create_translation(QM_FILES ${SRCS_TRANSLATIONS} ${TS_FILES})
#
# add_executable(myapp ${myapp_SRCS} ${QM_FILES})
#
# Copyright (C) 2008  Tanguy Krotoff <tkrotoff@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING file.


macro (clear_qt_translations)

	set(SRCS_TRANSLATIONS
		""
		CACHE INTERNAL "Sources (*.cpp, *.c...) to be translated" FORCE
	)

endmacro (clear_qt_translations)

macro (add_qt_translations)

	set(_fileList "")
	foreach(file ${ARGN})
		get_filename_component(_path ${file} ABSOLUTE)
		list(APPEND _fileList ${_path})
	endforeach(file ${ARGN})

	set(SRCS_TRANSLATIONS
		${SRCS_TRANSLATIONS}
		${_fileList}
		CACHE INTERNAL "Sources (*.cpp, *.c...) to be translated" FORCE
	)

endmacro (add_qt_translations)
