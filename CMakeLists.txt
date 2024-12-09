cmake_minimum_required( VERSION 3.30)

set( CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set( TARGET run)
set( DYLIB api_photoshop)

project( Photoshop)

file( GLOB_RECURSE SOURCES src/controller/*.cpp src/gui/*.cpp src/sys/*.cpp src/model/*.cpp, src/main.cpp)
file( GLOB_RECURSE SRC_DYLIB src/api_impl/*.cpp src/sys/my_exceptions.cpp)

add_library( ${DYLIB} SHARED ${SRC_DYLIB})
add_executable( ${TARGET} ${SOURCES})

target_link_directories( ${DYLIB} PRIVATE /opt/homebrew/lib/)
target_link_directories( ${DYLIB} PRIVATE build/)
target_link_libraries( ${DYLIB} PRIVATE sfml-graphics sfml-window sfml-system)

target_link_directories( ${TARGET} PRIVATE /opt/homebrew/lib/)
target_link_directories( ${TARGET} PRIVATE build/)
target_link_libraries( ${TARGET} PRIVATE sfml-graphics sfml-window sfml-system)
target_link_libraries( ${TARGET} PRIVATE api_photoshop)

include_directories( headers)
include_directories( ${CMAKE_SOURCE_DIR})
include_directories( /opt/homebrew/include/)
include_directories( ../MyLibraries/headers/)

# set(PLUGINS "canvas" "toolbar" "optionsbar" "brush" "eraser" "rectangle" "ellipse" "line"
# 			"brightness_filter" "negative_filter" "barelief_filter" "bluer_filter" "gauss_bluer_filter"
# 			"contrast_filter" "filter" "menubar")


set(LIBRARIES
    "canvas+${CMAKE_SOURCE_DIR}/plugins/canvas.cpp"
    "toolbar+${CMAKE_SOURCE_DIR}/plugins/toolbar/toolbar.cpp"
    "optionsbar+${CMAKE_SOURCE_DIR}/plugins/optionsbar/optionsbar.cpp"
    "brush+${CMAKE_SOURCE_DIR}/plugins/toolbar/brush.cpp"
    "eraser+${CMAKE_SOURCE_DIR}/plugins/toolbar/eraser.cpp"
    "rectangle+${CMAKE_SOURCE_DIR}/plugins/toolbar/rectangle.cpp"
    "ellipse+${CMAKE_SOURCE_DIR}/plugins/toolbar/ellipse.cpp"
    "line+${CMAKE_SOURCE_DIR}/plugins/toolbar/line.cpp"
    "menubar+${CMAKE_SOURCE_DIR}/plugins/menubar/menubar.cpp"
	"menu_items+${CMAKE_SOURCE_DIR}/plugins/menubar/menu_items.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/fill_menu.cpp,${CMAKE_SOURCE_DIR}/plugins/menubar/filters.cpp"
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

# add_compile_options( -ggdb3 -std=c++20 -O3 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-qual
# 		-Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline
# 		-Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo
# 		-Wstrict-overflow=2 -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros
# 		-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation
# 		-fstrict-overflow -fno-omit-frame-pointer -fPIE -Werror=vla -flat-namespace)
# -Wsuggest-override
add_compile_options( -O3 -std=c++20 -fcolor-diagnostics)

# add_compile_options( -fsanitize=address,alignment)
# add_link_options( -fsanitize=address,alignment)
