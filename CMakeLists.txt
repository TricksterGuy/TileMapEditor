cmake_minimum_required (VERSION 2.6)
project (tilemapeditor)

set(CPACK_BINARY_DEB "ON")
set(CPACK_BINARY_RPM "OFF")
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tile Map Editor")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "tilemapeditor 1.0.0")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_DEBIAN_PACKAGE_NAME "tilemapeditor")
set(CPACK_DEBIAN_PACKAGE_VERSION "1.0")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Brandon Whitehead <brandon.whitehead@gatech.edu>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "tile map editor")

set(CPACK_PACKAGE_FILE_NAME "tilemapeditor-1.0.0-i386")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")

include(CPack)

add_definitions(-DLINUX)

option(DEBUG_MODE "Build for debugging" ON)
option(TEST_MODE "Build for testing" OFF)

# Source files definition
set(SRC_TILEMAPEDITOR
    src/gen/TilemapEditorGUI.cpp
    src/gen/MapNotebookGUI.cpp

    src/gui/Clock.cpp
    src/gui/MapCanvas.cpp
    src/gui/MapDocument.cpp
    src/gui/MapView.cpp
    src/gui/MapViewUpdate.cpp
    src/gui/ParallaxBackground.cpp
    src/gui/TilemapEditorApp.cpp
    src/gui/TilemapEditorFrame.cpp
)

set(SRC_MAP
    src/data/AnimatedTile.cpp
    src/data/Background.cpp
    src/data/CollisionLayer.cpp
    src/data/DrawAttributes.cpp
    src/data/Layer.cpp
    src/data/Map.cpp
    src/data/PixelBasedCollisionLayer.cpp
    src/data/Rectangle.cpp
    src/data/Region.cpp
    src/data/TileBasedCollisionLayer.cpp
)

set(SRC_HANDLERS
    src/handlers/BaseMapHandler.cpp
    src/handlers/BinaryMapHandler.cpp
    src/handlers/GBAImageHandler.cpp
    src/handlers/GBAMapHandler.cpp
    src/handlers/HandlerUtils.cpp
    src/handlers/ImageMapHandler.cpp
    src/handlers/MapHandlerManager.cpp
    src/handlers/TextMapHandler.cpp
    src/handlers/XmlMapHandler.cpp
)

set(SRC_UTIL
    src/util/Scanner.cpp
    src/util/Logger.cpp
)

set(SRC_wxFlatNotebook
    lib/wxFlatNotebook/fnb_customize_dlg.cpp
    lib/wxFlatNotebook/fnb_resources.cpp
    lib/wxFlatNotebook/popup_dlg.cpp
    lib/wxFlatNotebook/renderer.cpp
    lib/wxFlatNotebook/wxFlatNotebook.cpp
    lib/wxFlatNotebook/xh_fnb.cpp
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(wxWidgets_USE_LIBS xrc xml html adv gl net core base)
find_package(wxWidgets REQUIRED)
find_package(ImageMagick COMPONENTS Magick++)
find_package(GTK2 REQUIRED gtk)
find_package(Boost 1.54 REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
pkg_search_module(PROTOBUF REQUIRED protobuf)
#execute_process(COMMAND sh "${wxWidgets_CONFIG_EXECUTABLE}" --cxxflags)

IF(DEBUG_MODE)
    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x -DwxUSE_GRAPHICS_CONTEXT=1")
ELSE()
    set(CMAKE_CXX_FLAGS "-O3 -s -Wall -std=c++0x -DwxUSE_GRAPHICS_CONTEXT=1")
ENDIF(DEBUG_MODE)

set(CMAKE_SKIP_RPATH TRUE)
include(${wxWidgets_USE_FILE})

include_directories(${tilemapeditor_SOURCE_DIR}/src/data)
include_directories(${tilemapeditor_SOURCE_DIR}/src/gen)
include_directories(${tilemapeditor_SOURCE_DIR}/src/gui)
include_directories(${tilemapeditor_SOURCE_DIR}/src/handlers)
include_directories(${tilemapeditor_SOURCE_DIR}/src/util)
include_directories(${tilemapeditor_SOURCE_DIR}/lib/wxFlatNotebook)
include_directories(${ImageMagick_INCLUDE_DIRS})
include_directories(${GTK2_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROTOBUF_INCLUDEDIR})

add_executable(
	tilemapeditor
    ${SRC_TILEMAPEDITOR}
)

add_library(
	wxFlatNotebook
	STATIC
	${SRC_wxFlatNotebook}
)

add_library(
    map
    STATIC
    ${SRC_MAP}
)

add_library(
    handlers
    STATIC
    ${SRC_HANDLERS}
)

add_library(
    util
    STATIC
    ${SRC_UTIL}
)

target_link_libraries(
	tilemapeditor
    handlers
    util
    map
	wxFlatNotebook
   	${wxWidgets_LIBRARIES}
    ${ImageMagick_LIBRARIES}
    ${GTK2_LIBRARIES}
    ${Boost_LIBRARIES}
)

add_executable(
    TileMapEditorTest
    src/testing/BasicHandlersTest.cpp
    src/testing/GlobalTest.cpp
    src/testing/LayerTest.cpp
    src/testing/MapTest.cpp
    src/testing/TextMapHandlerTest.cpp
    src/testing/TestUtil.cpp
    src/testing/XmlMapHandlerTest.cpp
)

target_link_libraries(
    TileMapEditorTest
    handlers
    util
    map
	${wxWidgets_LIBRARIES}
	${GTK2_LIBRARIES}
	${PROTOBUF_LIBRARIES}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tilemapeditor DESTINATION bin)
