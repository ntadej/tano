################
# Translations #
################
IF(NOT QT_LUPDATE_EXECUTABLE)
	MESSAGE(WARNING "Tano could not find lupdate. You won't be able to update translations.")
ENDIF(NOT QT_LUPDATE_EXECUTABLE)

IF(NOT QT_LRELEASE_EXECUTABLE)
	MESSAGE(WARNING "Tano could not find lrelease. Your build won't contain translations.")
ENDIF(NOT QT_LRELEASE_EXECUTABLE)

## Wraps the supplied .ts files in lrelease commands
MACRO(QT4_ADD_TRANSLATIONS outfiles)
	FOREACH (it ${ARGN})
		GET_FILENAME_COMPONENT(it ${it} ABSOLUTE)
		GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)

		SET(outfile ${CMAKE_BINARY_DIR}/lang/${outfile}.qm)

		ADD_CUSTOM_COMMAND(
			OUTPUT ${outfile}
			COMMAND ${QT_LRELEASE_EXECUTABLE}
			ARGS -compress -silent -nounfinished ${it} -qm ${outfile}
			MAIN_DEPENDENCY ${it}
		)

		SET(${outfiles} ${${outfiles}} ${outfile})
	ENDFOREACH(it)
ENDMACRO(QT4_ADD_TRANSLATIONS)

## Collect all translations files
FILE (GLOB Tano_TS ${CMAKE_SOURCE_DIR}/lang/*.ts)

IF(QT_LRELEASE_EXECUTABLE)
	## Wraps the collected translations
	QT4_ADD_TRANSLATIONS(Tano_QMS ${Tano_TS})

	## Create a target that runs lrelease for all the .ts files
	## "ALL" means that it will be run by default.
	ADD_CUSTOM_TARGET(translations ALL DEPENDS ${Tano_QMS})
ENDIF(QT_LRELEASE_EXECUTABLE)

IF(QT_LUPDATE_EXECUTABLE)
	## Search for all translatable strings in the sources directory
	FILE(GLOB_RECURSE translate_SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp)
	FILE(GLOB_RECURSE translate_HDRS ${CMAKE_SOURCE_DIR}/src/*.h)
	FILE(GLOB_RECURSE translate_UIS ${CMAKE_SOURCE_DIR}/src/*.ui)

	SET(translate_SRCS ${translate_SRCS} ${translate_HDRS} ${translate_UIS})

	## Create a target that runs lupdate for all the sources and UI files
	IF(REFRESH_TRANSLATIONS)
		ADD_CUSTOM_TARGET(lupdate ALL
			COMMAND ${QT_LUPDATE_EXECUTABLE} -no-obsolete ${translate_SRCS} -ts ${Tano_TS} )
	ELSE(REFRESH_TRANSLATIONS)
		ADD_CUSTOM_TARGET(lupdate
			COMMAND ${QT_LUPDATE_EXECUTABLE} -no-obsolete ${translate_SRCS} -ts ${Tano_TS} )
	ENDIF(REFRESH_TRANSLATIONS)
ENDIF(QT_LUPDATE_EXECUTABLE)


################
# Installation #
################
INSTALL(FILES ${Tano_QMS} DESTINATION "${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_DATA_DIR}/lang")
