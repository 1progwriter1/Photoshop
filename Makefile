DYLIB_DIR := build

all: build/lib_blur_filter.dylib
	g++ -shared -o $@ $^

	build/lib_blur_filter.dylib : plugins/blurFilter/blurFilter.cpp \
	plugins/pluginLib/windows/windows.cpp plugins/pluginLib/bars/ps_bar.cpp \
	plugins/pluginLib/canvas/canvas.cpp plugins/pluginLib/filters/filters.cpp libapi_photoshop.dylib
