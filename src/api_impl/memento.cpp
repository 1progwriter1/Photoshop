#include <api_impl/memento.hpp>
#include <cassert>



ALayerSnapshot::ALayerSnapshot(const std::vector<Color> pixels, vec2i offset)
    :   pixels_(std::move(pixels)), offset_(offset) {}


Color ALayerSnapshot::operator[](size_t index)
{
    return pixels_[index];
}


vec2i ALayerSnapshot::getOffset()
{
    return offset_;
}

template<>
std::unique_ptr<ILayerSnapshot> AMementable<ILayerSnapshot>::save()
{
    Canvas *canvas = dynamic_cast<Canvas *>(getRootWindow()->getWindowById(kCanvasWindowId));
    ILayer *layer = canvas->getLayer(canvas->getActiveLayerIndex());

    vec2u size = canvas->getActualRect().size;
    std::vector<Color> pixels(size.x * size.y);
    for ( int x = 0; x < size.x; x++ )
    {
        for ( int y = 0; y < size.y; y++ )
        {
            pixels[y * size.x + x] = layer->getPixel(vec2i(x, y));
        }
    }
    return std::make_unique<ALayerSnapshot>(std::move(pixels), canvas->getActualRect().pos - canvas->getPos());
}


template<>
void AMementable<ILayerSnapshot>::restore(ILayerSnapshot* snapshot)
{
    Canvas *canvas = dynamic_cast<Canvas *>(getRootWindow()->getWindowById(kCanvasWindowId));
    Layer *layer = dynamic_cast<Layer *>(canvas->getLayer(canvas->getActiveLayerIndex()));
    ALayerSnapshot *my_snapshot = dynamic_cast<ALayerSnapshot *>(snapshot);
    assert( my_snapshot && "Wrong snapshot type" );

    vec2i offset = my_snapshot->getOffset();
    vec2u size = canvas->getActualRect().size;
    for ( int x = 0; x < size.x; x++ )
    {
        for ( int y = 0; y < size.y; y++ )
        {
            layer->setPixelGlobal(vec2i(x + offset.x, y + offset.y), (*my_snapshot)[y * size.x + x]);
        }
    }
}


template<>
std::unique_ptr<ACanvasSnapshot> AMementable<ACanvasSnapshot>::save()
{
    assert( 0 && "Not implemented" );
    return nullptr;
}


template<>
void AMementable<ACanvasSnapshot>::restore(ACanvasSnapshot* snapshot)
{
    assert( 0 && "Not implemented" );
}


