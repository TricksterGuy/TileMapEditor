cmake_minimum_required (VERSION 2.6)
if(CMAKE_VERSION VERSION_EQUAL 3.0 OR CMAKE_VERSION VERSION_GREATER 3.0)
    # Ignore warnings about rpath behavior on OS X
    cmake_policy(SET CMP0042 NEW)
endif()
project (tilemapeditor)
include(FindPkgConfig)


# Require C++11 build
if(NOT CMAKE_CXX_FLAGS MATCHES "-std=(c|gnu)\\+\\+11")
    message(STATUS "This project requires C++11. Adding -std=c++11 to CXXFLAGS.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

# Additional debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -pg -fprofile-arcs -ftest-coverage --coverage -DDEBUG")
# Install directories
set(CMAKE_INSTALL_LIBDIR lib CACHE STRING "where to install libraries/plugins")
set(CMAKE_INSTALL_BINDIR bin CACHE STRING "where to install programs")

# Set default warning flags
set(PROJECT_WARNING_FLAGS "-Wall -Wno-deprecated -Wno-deprecated-declarations" CACHE STRING "Compiler warning flags to include")
mark_as_advanced(PROJECT_WARNING_FLAGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROJECT_WARNING_FLAGS}")

# Default to release build if not specified
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

add_definitions(-DPREFIX=${CMAKE_INSTALL_PREFIX})
add_definitions(-DSO_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}")

if(CMAKE_HOST_UNIX)
add_definitions(-DLINUX)
endif(CMAKE_HOST_UNIX)

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
    src/data/DrawAttributes.cpp
    src/data/Layer.cpp
    src/data/Map.cpp
    src/data/PixelBasedCollisionLayer.cpp
    src/data/Rectangle.cpp
    src/data/Region.cpp
    src/data/TileBasedCollisionLayer.cpp
    src/data/Tileset.cpp
)

set(SRC_HANDLERS
    src/handlers/BaseMapHandler.cpp
    src/handlers/BinaryMapHandler.cpp
    #src/handlers/GBAImageHandler.cpp
    #src/handlers/GBAMapHandler.cpp
    #src/handlers/HandlerUtils.cpp
    #src/handlers/ImageMapHandler.cpp
    src/handlers/MapHandlerManager.cpp
    src/handlers/TextMapHandler.cpp
    #src/handlers/XmlMapHandler.cpp
)

set(SRC_UTIL
    src/util/Scanner.cpp
    src/util/Logger.cpp
    src/util/ChunkStream.cpp
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
find_package(Boost 1.54 REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
pkg_check_modules(ImageMagick Magick++ MagickWand MagickCore)
pkg_search_module(PROTOBUF REQUIRED protobuf)

add_definitions("-DwxUSE_GRAPHICS_CONTEXT=1")

set(CMAKE_SKIP_RPATH TRUE)
include(${wxWidgets_USE_FILE})

include_directories(${tilemapeditor_SOURCE_DIR}/src/data)
include_directories(${tilemapeditor_SOURCE_DIR}/src/gen)
include_directories(${tilemapeditor_SOURCE_DIR}/src/gui)
include_directories(${tilemapeditor_SOURCE_DIR}/src/handlers)
include_directories(${tilemapeditor_SOURCE_DIR}/src/util)
include_directories(${tilemapeditor_SOURCE_DIR}/lib/wxFlatNotebook)
include_directories(${ImageMagick_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROTOBUF_INCLUDEDIR})

add_definitions(-DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16)


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
    ${Boost_LIBRARIES}
)

add_executable(
    TileMapEditorTest
    src/testing/BasicHandlersTest.cpp
    src/testing/GlobalTest.cpp
    src/testing/LayerTest.cpp
    src/testing/MapTest.cpp
    src/testing/RegionTest.cpp
    #src/testing/TextMapHandlerTest.cpp
    src/testing/TestUtil.cpp
    #src/testing/XmlMapHandlerTest.cpp
    src/testing/ChunkStreamTest.cpp
)

target_link_libraries(
    TileMapEditorTest
    handlers
    util
    map
	${wxWidgets_LIBRARIES}
	${PROTOBUF_LIBRARIES}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tilemapeditor DESTINATION bin)
