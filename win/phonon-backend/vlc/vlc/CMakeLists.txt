project(phonon-vlc)

find_package(VLC)
if (VLC_FOUND)
	include_directories(${VLC_INCLUDE_DIRS})
else (VLC_FOUND)
	message(STATUS "libvlc external support disabled, revert to internal libvlc.h")
	#Do nothing, use internal libvlc.h, already in the include path
endif (VLC_FOUND)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
	#stdint.h is not shipped with Visual C++
	#see http://en.wikipedia.org/wiki/Stdint.h
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vlc/msvc)
endif (MSVC)

add_definitions(-DPHONON_VLC)

set(phonon_vlc_SRCS
	${phonon_commom_SRCS}
	vlc_loader.cpp
	VLCMediaObject.cpp
	VLCMediaController.cpp
	VLCVideoWidget.cpp
	vlc_symbols.cpp
	win32currentdirectory.cpp
)

if (KDE4_FOUND)
	kde4_add_plugin(phonon_vlc ${phonon_vlc_SRCS})

	target_link_libraries(phonon_vlc
		${KDE4_KDECORE_LIBS}
		${KDE4_KDEUI_LIBS}
		${QT_PHONON_LIBRARY}
	)
else (KDE4_FOUND)
	qt4_wrap_cpp(phonon_vlc_SRCS
		${phonon_commom_MOCS}
		VLCMediaObject.h
		VLCVideoWidget.h
	)

	add_library(phonon_vlc MODULE ${phonon_vlc_SRCS})

	target_link_libraries(phonon_vlc
		${QT_QTCORE_LIBRARY}
		${QT_QTGUI_LIBRARY}
		${QT_PHONON_LIBRARY}
	)
endif (KDE4_FOUND)

if (VLC_FOUND)
	target_link_libraries(phonon_vlc
		${VLC_LIBRARIES}
	)
endif (VLC_FOUND)

if (KDE4_FOUND)
	install(TARGETS phonon_vlc DESTINATION ${PLUGIN_INSTALL_DIR})
	install(FILES vlc.desktop DESTINATION ${SERVICES_INSTALL_DIR}/phononbackends)
	install(FILES icons/16x16/phonon-vlc.png DESTINATION ${ICON_INSTALL_DIR}/oxygen/16x16/apps)
	install(FILES icons/32x32/phonon-vlc.png DESTINATION ${ICON_INSTALL_DIR}/oxygen/32x32/apps)
	install(FILES icons/48x48/phonon-vlc.png DESTINATION ${ICON_INSTALL_DIR}/oxygen/48x48/apps)
	install(FILES icons/128x128/phonon-vlc.png DESTINATION ${ICON_INSTALL_DIR}/oxygen/128x128/apps)

	#Ubuntu 8.04 Hardy
	install(TARGETS phonon_vlc DESTINATION /usr/lib/kde4/lib/kde4)
	install(FILES vlc.desktop DESTINATION /usr/lib/kde4/share/kde4/services/phononbackends)
	install(FILES icons/16x16/phonon-vlc.png DESTINATION /usr/lib/kde4/share/icons/oxygen/16x16/apps)
	install(FILES icons/32x32/phonon-vlc.png DESTINATION /usr/lib/kde4/share/icons/oxygen/32x32/apps)
	install(FILES icons/48x48/phonon-vlc.png DESTINATION /usr/lib/kde4/share/icons/oxygen/48x48/apps)
	install(FILES icons/128x128/phonon-vlc.png DESTINATION /usr/lib/kde4/share/icons/oxygen/128x128/apps)
else (KDE4_FOUND)
	install(TARGETS phonon_vlc DESTINATION ${PLUGIN_INSTALL_DIR})
	if (WIN32)
		#install(TARGETS phonon_vlc DESTINATION ${QT_PLUGINS_DIR}/phonon_backend)
	endif (WIN32)
endif (KDE4_FOUND)
