cmake_minimum_required( VERSION 3.10)

set( CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set( TARGET run)
set( DYLIB api_photoshop)

project( Photoshop)

file( GLOB_RECURSE SOURCES src/controller/*.cpp src/gui/*.cpp src/sys/*.cpp src/model/*.cpp, src/main.cpp)
file( GLOB_RECURSE SRC_DYLIB src/api_impl/*.cpp src/sys/my_exceptions.cpp)

add_library( ${DYLIB} SHARED ${SRC_DYLIB})
add_executable( ${TARGET} ${SOURCES})


set(PLUGIN_INSTALL_DIR /Users/ivanvodolazenko/VSProjects/Photoshop/plugins/)

function( add_dynamic_plugin plugin_name)
	set( plugin_src "../plugins/${plugin_name}.cpp")
	if(NOT EXISTS ${plugin_src})
        message(FATAL_ERROR "Source file ${plugin_scr} does not exist.")
    endif()
	add_library( ${plugin_name} SHARED ${plugin_src})

	# set_target_properties( ${plugin_name} PROPERTIES OUTPUT_NAME "${plugin_name}")

	target_link_directories( ${plugin_name} PRIVATE /opt/homebrew/lib/)
	target_link_libraries( ${plugin_name} PRIVATE sfml-graphics sfml-window sfml-system)
	target_link_directories( ${plugin_name} PRIVATE build/)
	target_link_libraries( ${plugin_name} PRIVATE api_photoshop)
endfunction()

set(PLUGINS "canvas" "toolbar" "optionsbar" "brush" "eraser" "rectangle" "ellipse" "line"
			"brightness_filter" "negative_filter" "barelief_filter" "bluer_filter" "gauss_bluer_filter"
			"contrast_filter")
foreach(plugin ${PLUGINS})
    add_dynamic_plugin(${plugin})
endforeach()






target_link_directories( ${DYLIB} PRIVATE /opt/homebrew/lib/)
target_link_directories( ${DYLIB} PRIVATE build/)
target_link_libraries( ${DYLIB} PRIVATE sfml-graphics sfml-window sfml-system)

target_link_directories( ${TARGET} PRIVATE /opt/homebrew/lib/)
target_link_directories( ${TARGET} PRIVATE build/)
target_link_libraries( ${TARGET} PRIVATE sfml-graphics sfml-window sfml-system)
target_link_libraries( ${TARGET} PRIVATE api_photoshop)

include_directories( headers)
include_directories( api)
include_directories( /opt/homebrew/include/)
include_directories( ../MyLibraries/headers/)

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
