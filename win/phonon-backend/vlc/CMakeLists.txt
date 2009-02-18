project(phonon-vlc-mplayer)

#Copy config.h to the current source directory
#since build dir in not inside the include path
#If it needed, this can be changed
configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

#phonon-common
set(phonon_commom_SRCS
	../Backend.cpp
	../MediaObject.cpp
	../MediaController.cpp
	../VideoWidget.cpp
	../AudioOutput.cpp
	../EffectManager.cpp
	../Effect.cpp
	../SinkNode.cpp
	../SeekStack.cpp
	../WidgetNoPaintEvent.cpp
)

set(phonon_commom_MOCS
	../Backend.h
	../MediaObject.h
	../VideoWidget.h
	../AudioOutput.h
	../EffectManager.h
	../Effect.h
	../SinkNode.h
	../SeekStack.h
	../WidgetNoPaintEvent.h
)
##

add_subdirectory(vlc)
add_subdirectory(mplayer)
