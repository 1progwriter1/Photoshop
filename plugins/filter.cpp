#include "filter.hpp"
#include <cassert>
#include <iostream>


#include "../headers/windows_id.hpp"
#include "../headers/api_impl/canvas.hpp"


psapi::IWindowContainer *kRootWindowPtr = nullptr;


const std::array<float, 9> GAUSS_MATRIX =
{
    0.025, 0.1, 0.025,
    0.1, 0.5, 0.1,
    0.025, 0.1, 0.025
};
const float GAUSS_SUM = 1;


bool loadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::Texture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::Sprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../images/filter48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( 36, 516);

    std::unique_ptr<ABarButton> filter = std::make_unique<FilterButton>( kGaussBluerFilterButtonId, texture, sprite);
    filter->setParent( toolbar);
    toolbar->addWindow( std::move( filter));

    return true;
}

void unloadPlugin()
{

}

FilterButton::FilterButton( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite), options_bar_( dynamic_cast<IBar*>( kRootWindowPtr->getWindowById( kOptionsBarWindowId)))
{
    assert( options_bar_ && "Failed to cast to options bar" );
}


bool FilterButton::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( getState() != IBarButton::State::Press )
    {
        if ( options_added_ )
        {
            removeOptions();
            options_added_ = false;
        }
        return true;
    }
    if ( !options_added_ )
    {
        createOptions();
        addOptions();
        options_added_ = true;
    }

    return true;
}

void FilterButton::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
}


void FilterButton::addOptions()
{
    for ( auto &option : options_ )
    {
        options_bar_->addWindow( std::move( option));
    }
    options_.clear();
}


void FilterButton::removeOptions()
{
    for ( auto &id : id_ )
    {
        options_bar_->removeWindow( id);
    }
    id_.clear();
}



void FilterButton::createOptions()
{
    sfm::vec2i button_pos = options_bar_->getPos() + sfm::vec2i( 26, 26);

    std::unique_ptr<sfm::Texture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::Sprite> sprite = std::make_unique<sfm::Sprite>();

    texture->loadFromFile("../images/contrast_filter48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( button_pos.x, button_pos.y);

    std::unique_ptr<ABarButton> button = std::make_unique<ContrastFilter>( kRedColorButtonId, texture, sprite);
    options_.push_back( std::move( button));
    id_.push_back( kContrastFilterButtonId);

    button_pos += sfm::vec2i( 96, 0);

    texture = std::make_unique<sfm::Texture>();
    sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../images/gauss_bluer_filter48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( button_pos.x, button_pos.y);

    button = std::make_unique<GaussBluerFilter>( kGaussBluerFilterButtonId, texture, sprite);
    options_.push_back( std::move( button));
    id_.push_back( kGaussBluerFilterButtonId);

    button_pos += sfm::vec2i( 96, 0);

    texture = std::make_unique<sfm::Texture>();
    sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../images/barelief_filter48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( button_pos.x, button_pos.y);

    button = std::make_unique<BareliefFilter>( kBareliefFilterButtonId, texture, sprite);
    options_.push_back( std::move( button));
    id_.push_back( kBareliefFilterButtonId);
}


ContrastFilter::ContrastFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   AFilter( init_id, init_texture, init_sprite), canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
}

AFilter::AFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite)
{}

GaussBluerFilter::GaussBluerFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   AFilter( init_id, init_texture, init_sprite), canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
}


BareliefFilter::BareliefFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   AFilter( init_id, init_texture, init_sprite), canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
}


void ContrastFilter::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
}


bool ContrastFilter::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( getState() != IBarButton::State::Press )
    {
        return true;
    }

    ILayer *layer = canvas_->getLayer( canvas_->getActiveLayerIndex());
    assert( layer );
    assert( canvas_->insertEmptyLayer( 1) );
    ILayer *new_layer = canvas_->getLayer( 1);
    assert( new_layer );
    assert( new_layer != layer );

    sfm::vec2u canvas_size = canvas_->getSize();

    for ( size_t x = 0; x < canvas_size.x - 2; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y - 2; y++ )
        {
            int from_x = std::max( 0, static_cast<int>( x - 1));
            int to_x = std::min( static_cast<int>( canvas_size.x - 1), static_cast<int>( x + 1));

            int from_y = std::max( 0, static_cast<int>( y - 1));
            int to_y = std::min( static_cast<int>( canvas_size.y - 1), static_cast<int>( y + 1));

            float r = 0, g = 0, b = 0, a = 0;

            for ( int i = from_x; i <= to_x; i++ )
            {
                for ( int j = from_y; j <= to_y; j++ )
                {
                    sfm::Color color = layer->getPixel( sfm::vec2i( i, j));
                    r += color.r * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    g += color.g * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    b += color.b * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    a += color.a * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                }
            }
            r /= GAUSS_SUM;
            g /= GAUSS_SUM;
            b /= GAUSS_SUM;
            a /= GAUSS_SUM;

            sfm::Color color( static_cast<uint8_t>( r), static_cast<uint8_t>( g), static_cast<uint8_t>( b), static_cast<uint8_t>( a));

            new_layer->setPixel( sfm::vec2i( x, y), color);
        }
    }
    for ( int x = 0; x < canvas_size.x; x++ )
    {
        for ( int y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color normal = layer->getPixel( sfm::vec2i( x, y));
            sfm::Color blurred = new_layer->getPixel( sfm::vec2i( x, y));
            layer->setPixel( sfm::vec2i(x, y), createContrastColor( normal, blurred));
        }
    }
    canvas_->removeLayer( 1);
    setState( IBarButton::State::Normal);

    return true;
}


sfm::Color createContrastColor( const sfm::Color &normal, const sfm::Color &blurred)
{
    float r = normal.r / 255.f + (normal.r - blurred.r) * 2 / 255.f;
    float g = normal.g / 255.f + (normal.g - blurred.g) * 2 / 255.f;
    float b = normal.b / 255.f + (normal.b - blurred.b) * 2 / 255.f;

    float max_c = std::max( std::max( r, g), b);

    float r_c = r / max_c * 255.f;
    float g_c = g / max_c * 255.f;
    float b_c = b / max_c * 255.f;

    return sfm::Color( static_cast<uint8_t>( r_c), static_cast<uint8_t>( g_c), static_cast<uint8_t>( b_c));
}


void GaussBluerFilter::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
}


bool GaussBluerFilter::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( getState() != IBarButton::State::Press )
    {
        return true;
    }

    ILayer *layer = canvas_->getLayer( canvas_->getActiveLayerIndex());
    assert( layer );

    sfm::vec2u canvas_size = canvas_->getSize();

    for ( size_t x = 0; x < canvas_size.x - 2; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y - 2; y++ )
        {
            int from_x = std::max( 0, static_cast<int>( x - 1));
            int to_x = std::min( static_cast<int>( canvas_size.x - 1), static_cast<int>( x + 1));

            int from_y = std::max( 0, static_cast<int>( y - 1));
            int to_y = std::min( static_cast<int>( canvas_size.y - 1), static_cast<int>( y + 1));

            int r = 0, g = 0, b = 0, a = 0;

            for ( int i = from_x; i <= to_x; i++ )
            {
                for ( int j = from_y; j <= to_y; j++ )
                {
                    sfm::Color color = layer->getPixel( sfm::vec2i( i, j));
                    r += color.r;
                    g += color.g;
                    b += color.b;
                    a += color.a;
                }
            }
            r /= 9;
            g /= 9;
            b /= 9;
            a /= 9;

            sfm::Color color( r, g, b, a);

            layer->setPixel( sfm::vec2i( x, y), color);
        }
    }

    setState( IBarButton::State::Normal);

    return true;
}


void BareliefFilter::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
}


bool BareliefFilter::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( getState() != IBarButton::State::Press )
    {
        return true;
    }

    ILayer *layer = canvas_->getLayer( canvas_->getActiveLayerIndex());
    assert( layer );
    canvas_->insertEmptyLayer( 1);
    ILayer *new_layer = canvas_->getLayer( 1);
    assert( new_layer );
    assert( new_layer != layer );

    sfm::vec2u canvas_size = canvas_->getSize();

    for ( unsigned x = 0; x < canvas_size.x; x++ )
    {
        for ( unsigned y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color_1 = layer->getPixel( sfm::vec2i( std::min( x + 4, canvas_size.x - 1), y));
            sfm::Color color_2 = layer->getPixel( sfm::vec2i( x, std::min( y + 4, canvas_size.y - 1)));

            sfm::Color color;
            color.r = (255 - color_1.r + color_2.r) / 2;
            color.g = (255 - color_1.g + color_2.g) / 2;
            color.b = (255 - color_1.b + color_2.b) / 2;
            color.a = (255 - color_1.a + color_2.a) / 2;
            new_layer->setPixel( sfm::vec2i( x, y), color);
        }
    }

    canvas_->removeLayer( 0);

    setState( IBarButton::State::Normal);

    return true;
}
