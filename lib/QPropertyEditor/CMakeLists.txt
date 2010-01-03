PROJECT(QPropertyEditor)

IF(MINGW)
add_definitions(-DCMAKE_TOOLCHAIN=../../Toolchain-win32.cmake)
ENDIF(MINGW)

# Set your list of sources here.
SET(QPROPERTY_EDITOR_SRCS
    ColorCombo.cpp
    EnumProperty.cpp
    Property.cpp
    QPropertyEditorWidget.cpp
    QPropertyModel.cpp
    QVariantDelegate.cpp
)

SET(QPROPERTY_EDITOR_MOCS
    ColorCombo.h
    EnumProperty.h
    Property.h
    QPropertyEditorWidget.h
    QPropertyModel.h
    QVariantDelegate.h
)

# Search for Qt4. We need the latest version of Qt
SET(QT_MIN_VERSION "4.4.0")
# this line includes FindQt4.cmake, which searches the Qt library and headers
FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE(${QT_USE_FILE})
QT4_WRAP_CPP(QPROPERTY_EDITOR_MOCS_SRCS ${QPROPERTY_EDITOR_MOCS})

# Create the library.
ADD_LIBRARY(${PROJECT_NAME} STATIC ${QPROPERTY_EDITOR_SRCS} ${QPROPERTY_EDITOR_MOCS_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${QT_LIBRARIES})
