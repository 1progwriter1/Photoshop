#ifndef PLUGINS_BLUR_FILTER_BLUR_FILTER_HPP
#define PLUGINS_BLUR_FILTER_BLUR_FILTER_HPP

//g++ -shared -I./ -I./plugins/ -o build/lib_blur_filter.dylib plugins/blurFilter/blurFilter.cpp plugins/pluginLib/windows/windows.cpp plugins/pluginLib/bars/ps_bar.cpp plugins/pluginLib/canvas/canvas.cpp plugins/pluginLib/filters/filters.cpp -std=c++20 -Wno-narrowing build/libapi_photoshop.dylib

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

#endif // PLUGINS_BLUR_FILTER_BLUR_FILTER_HPP
