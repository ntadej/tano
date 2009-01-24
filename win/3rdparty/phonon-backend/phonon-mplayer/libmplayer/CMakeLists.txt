project(libmplayer)

set(libmplayer_SRCS
	MyProcess.cpp
	MediaData.cpp
	MediaSettings.cpp
	MPlayerVersion.cpp
	MPlayerProcess.cpp
	MPlayerLoader.cpp
)

qt4_wrap_cpp(libmplayer_SRCS
	MyProcess.h
	MPlayerProcess.h
)

add_library(libmplayer STATIC ${libmplayer_SRCS})

target_link_libraries(libmplayer
	${QT_QTCORE_LIBRARY}
	${QT_QTGUI_LIBRARY}
	${QT_PHONON_LIBRARY}
)
