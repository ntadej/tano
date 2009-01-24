add_subdirectory(libmplayer)

project(phonon_mplayer)

#Copy config.h to the current source directory
#since build dir in not inside the include path
#If it needed, this can be changed
configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

#Includes current source directory
#This allows us to write #include <libmplayer/..>
#instead of #include "libmplayer/.."
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(phonon_mplayer_SRCS
	Backend.cpp
	MediaObject.cpp
	MediaController.cpp
	VideoWidget.cpp
	AudioOutput.cpp
	EffectManager.cpp
	Effect.cpp
	SinkNode.cpp
	SeekStack.cpp
	WidgetNoPaintEvent.cpp
	MPlayerVideoWidget.cpp
)

qt4_wrap_cpp(phonon_mplayer_SRCS
	Backend.h
	MediaObject.h
	MediaController.h
	VideoWidget.h
	AudioOutput.h
	EffectManager.h
	Effect.h
	SinkNode.h
	SeekStack.h
	WidgetNoPaintEvent.h
	MPlayerVideoWidget.h
)

add_library(phonon_mplayer MODULE ${phonon_mplayer_SRCS})

if (KDE4_FOUND)
	target_link_libraries(phonon_mplayer
		${KDE4_KDECORE_LIBS}
		${KDE4_KDEUI_LIBS}
		${QT_PHONON_LIBRARY}
		libmplayer
	)
else (KDE4_FOUND)
	target_link_libraries(phonon_mplayer
		${QT_QTCORE_LIBRARY}
		${QT_QTGUI_LIBRARY}
		${QT_PHONON_LIBRARY}
		libmplayer
	)
endif (KDE4_FOUND)

if (KDE4_FOUND)
	#install(TARGETS phonon_mplayer DESTINATION ${PLUGIN_INSTALL_DIR})
	#install(FILES mplayer.desktop DESTINATION ${SERVICES_INSTALL_DIR}/phononbackends)
	#install(FILES icons/16x16/phonon-mplayer.png DESTINATION ${ICON_INSTALL_DIR}/oxygen/16x16/apps)
	#install(FILES icons/22x22/phonon-mplayer.png DESTINATION ${ICON_INSTALL_DIR}/oxygen/22x22/apps)
	#install(FILES icons/32x32/phonon-mplayer.png DESTINATION ${ICON_INSTALL_DIR}/oxygen/32x32/apps)
	#install(FILES icons/48x48/phonon-mplayer.png DESTINATION ${ICON_INSTALL_DIR}/oxygen/48x48/apps)

	# Ubuntu 8.04 Hardy
	#install(TARGETS phonon_mplayer DESTINATION /usr/lib/kde4/lib/kde4)
	#install(FILES mplayer.desktop DESTINATION /usr/lib/kde4/share/kde4/services/phononbackends)
	#install(FILES icons/16x16/phonon-mplayer.png DESTINATION /usr/lib/kde4/share/icons/oxygen/16x16/apps)
	#install(FILES icons/22x22/phonon-mplayer.png DESTINATION /usr/lib/kde4/share/icons/oxygen/22x22/apps)
	#install(FILES icons/32x32/phonon-mplayer.png DESTINATION /usr/lib/kde4/share/icons/oxygen/32x32/apps)
	#install(FILES icons/48x48/phonon-mplayer.png DESTINATION /usr/lib/kde4/share/icons/oxygen/48x48/apps)
else (KDE4_FOUND)
	install(TARGETS phonon_mplayer DESTINATION ${BIN_INSTALL_DIR}/phonon_backend)
	if (WIN32)
		#install(TARGETS phonon_mplayer DESTINATION ${QT_PLUGINS_DIR}/phonon_backend)
	endif (WIN32)
endif (KDE4_FOUND)
