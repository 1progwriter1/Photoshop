cmake_minimum_required( VERSION 3.10)

set( CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set( TARGET run)

project( Photoshop)

file( GLOB_RECURSE SOURCES src/*.cpp)


add_executable( ${TARGET} ${SOURCES})

target_link_directories( ${TARGET} PRIVATE /opt/homebrew/lib/)
target_link_libraries( ${TARGET} PRIVATE sfml-graphics sfml-window sfml-system)

include_directories( headers)
include_directories( api)
include_directories( /opt/homebrew/include/)
include_directories( ../MyLibraries/headers/)

add_compile_options( -D _DEBUG -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-qual
		-Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline
		-Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo
		-Wstrict-overflow=2 -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros
		-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation
		-fstack-protector -fstrict-overflow -fno-omit-frame-pointer -fPIE -Werror=vla)
# -Wsuggest-override

add_compile_options( -fsanitize=address,alignment)
add_link_options( -fsanitize=address,alignment)
