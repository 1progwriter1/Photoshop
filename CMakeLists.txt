cmake_minimum_required( VERSION 3.28.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set( TARGET run)
set( DYLIB api_photoshop)

project( Photoshop)

file( GLOB_RECURSE SOURCES src/controller/*.cpp src/gui/*.cpp src/sys/*.cpp src/model/*.cpp, src/main.cpp)
file( GLOB_RECURSE SRC_DYLIB src/api_impl/*.cpp src/sys/my_exceptions.cpp)

add_library( ${DYLIB} SHARED ${SRC_DYLIB})
add_executable( ${TARGET} ${SOURCES})

target_link_directories( ${DYLIB} PRIVATE build/)
target_link_libraries( ${DYLIB} PRIVATE sfml-graphics sfml-window sfml-system)

target_link_directories( ${TARGET} PRIVATE build/)
target_link_libraries( ${TARGET} PRIVATE sfml-graphics sfml-window sfml-system)
target_link_libraries( ${TARGET} PRIVATE api_photoshop)

include_directories( headers)
include_directories( ${CMAKE_SOURCE_DIR})
include_directories( ../MyLibraries/headers/)


set(LIBRARIES
    "canvas+${CMAKE_SOURCE_DIR}/plugins/canvas.cpp"
    "toolbar+${CMAKE_SOURCE_DIR}/plugins/toolbar/toolbar.cpp"
    "optionsbar+${CMAKE_SOURCE_DIR}/plugins/optionsbar/optionsbar.cpp"
    "brush+${CMAKE_SOURCE_DIR}/plugins/toolbar/brush.cpp"
    "ps_button+${CMAKE_SOURCE_DIR}/plugins/toolbar/ps_button.cpp"
    "eraser+${CMAKE_SOURCE_DIR}/plugins/toolbar/eraser.cpp"
    "rectangle+${CMAKE_SOURCE_DIR}/plugins/toolbar/rectangle.cpp"
    "ellipse+${CMAKE_SOURCE_DIR}/plugins/toolbar/ellipse.cpp"
    "line+${CMAKE_SOURCE_DIR}/plugins/toolbar/line.cpp"
    "menubar+${CMAKE_SOURCE_DIR}/plugins/menubar/menubar.cpp"
	"menu_items+${CMAKE_SOURCE_DIR}/plugins/menubar/menu_items.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/fill_menu.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/file.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/tools.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/layer.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/filters/blur.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/filters/unsharp_mask.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/filters/barelief.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/control_panel.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/filters/negative.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/filters/brightness.cpp"
    "gauss_blur+${CMAKE_SOURCE_DIR}/plugins/menubar/filters/gauss_blur.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/menu_items.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/control_panel.cpp"
)


foreach(LIB_DATA ${LIBRARIES})

	string(REPLACE "+" ";" PARTS ${LIB_DATA})
	list(LENGTH PARTS NUM_PARTS)

    if(NOT NUM_PARTS EQUAL 2)
        message(FATAL_ERROR "LIBRARIES format is incorrect: ${LIB_DATA}")
    endif()

    list(GET PARTS 0 LIB_NAME)
    list(GET PARTS 1 SRC_FILES)

    string(REPLACE "," ";" SRC_FILES ${SRC_FILES})

    foreach(SRC_FILE ${SRC_FILES})
        list(APPEND SRC_FILE_LIST ${SRC_FILE})
    endforeach()

    add_library(${LIB_NAME} SHARED ${SRC_FILE_LIST})
	target_link_directories( ${LIB_NAME} PRIVATE /opt/homebrew/lib/)
	target_link_directories( ${LIB_NAME} PRIVATE build/)
	target_link_libraries( ${LIB_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
	target_link_libraries( ${LIB_NAME} PRIVATE api_photoshop)

    foreach( SRC_FILE ${SRC_FILE_LIST})
        list(REMOVE_ITEM SRC_FILE_LIST ${SRC_FILE})
    endforeach()

endforeach()

add_compile_options( -O3 -fcolor-diagnostics)
