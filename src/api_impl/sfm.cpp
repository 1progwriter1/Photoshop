#include <api_impl/sfm.hpp>
#include <assert.h>


namespace psapi
{


namespace sfm
{


RenderWindow::RenderWindow( unsigned int width, unsigned int height, const std::string& title)
    : window_( sf::VideoMode(width, height), title) {}


void RenderWindow::setFps( float fps)
{
    assert( 0 && "Not implemented" );
}


float RenderWindow::getFps() const
{
    assert( 0 && "Not implemented" );

    return 0;
}


bool RenderWindow::isOpen() const
{
    return window_.isOpen();
}


void RenderWindow::clear()
{
    window_.clear();
}


void RenderWindow::display()
{
    window_.display();
}


void RenderWindow::close()
{
    window_.close();
}


sf::RenderWindow &RenderWindow::getWindow()
{
    return window_;
}


vec2u RenderWindow::getSize() const
{
    return vec2u( width_, height_);
}


bool RenderWindow::pollEvent( Event& event)
{
    sf::Event sfEvent;

    if ( !window_.pollEvent(sfEvent) )
    {
        return false;
    }

    switch ( sfEvent.type )
    {
        case sf::Event::Closed:
            event.type = Event::Closed;
            break;

        case sf::Event::Resized:
            event.type = Event::Resized;
            event.size.width  = sfEvent.size.width;
            event.size.height = sfEvent.size.height;
            break;

        case sf::Event::LostFocus:
            event.type = Event::LostFocus;
            break;

        case sf::Event::GainedFocus:
            event.type = Event::GainedFocus;
            break;

        case sf::Event::TextEntered:
            event.type = Event::TextEntered;
            event.text.unicode = sfEvent.text.unicode;
            break;

        case sf::Event::KeyPressed:
            event.type = Event::KeyPressed;
            event.key.code = static_cast<Keyboard::Key>(sfEvent.key.code);
            event.key.alt     = sfEvent.key.alt;
            event.key.control = sfEvent.key.control;
            event.key.shift   = sfEvent.key.shift;
            event.key.system  = sfEvent.key.system;
            break;

        case sf::Event::KeyReleased:
            event.type = Event::KeyReleased;
            event.key.code = static_cast<Keyboard::Key>(sfEvent.key.code);
            event.key.alt     = sfEvent.key.alt;
            event.key.control = sfEvent.key.control;
            event.key.shift   = sfEvent.key.shift;
            event.key.system  = sfEvent.key.system;
            break;

        case sf::Event::MouseWheelScrolled:
            event.type = Event::MouseWheelScrolled;
            event.mouseWheel.delta = sfEvent.mouseWheelScroll.delta;
            event.mouseWheel.x     = sfEvent.mouseWheelScroll.x;
            event.mouseWheel.y     = sfEvent.mouseWheelScroll.y;
            break;

        case sf::Event::MouseButtonPressed:
            event.type = Event::MouseButtonPressed;
            event.mouseButton.button = static_cast<Mouse::Button>(sfEvent.mouseButton.button);
            event.mouseButton.x = sfEvent.mouseButton.x;
            event.mouseButton.y = sfEvent.mouseButton.y;
            break;

        case sf::Event::MouseButtonReleased:
            event.type = Event::MouseButtonReleased;
            event.mouseButton.button = static_cast<Mouse::Button>(sfEvent.mouseButton.button);
            event.mouseButton.x = sfEvent.mouseButton.x;
            event.mouseButton.y = sfEvent.mouseButton.y;
            break;

        case sf::Event::MouseMoved:
            event.type = Event::MouseMoved;
            event.mouseMove.x = sfEvent.mouseMove.x;
            event.mouseMove.y = sfEvent.mouseMove.y;
            break;

        case sf::Event::MouseEntered:
            event.type = Event::MouseEntered;
            break;

        case sf::Event::MouseLeft:
            event.type = Event::MouseLeft;
            break;

        case sf::Event::MouseWheelMoved: // deprecated
        case sf::Event::JoystickButtonPressed:
        case sf::Event::JoystickButtonReleased:
        case sf::Event::JoystickMoved:
        case sf::Event::JoystickConnected:
        case sf::Event::JoystickDisconnected:
        case sf::Event::TouchBegan:
        case sf::Event::TouchMoved:
        case sf::Event::TouchEnded:
        case sf::Event::SensorChanged:
        case sf::Event::Count:
        default:
            event.type = Event::Unknown;
            break;
    }

    return true;
}

void RenderWindow::draw( Drawable *target)
{
    if (target)
    {
        target->draw( this);
    }
}


PixelsArray::PixelsArray( size_t init_size)
    :   pixels_( sf::Points, init_size), size_( init_size) {}


void PixelsArray::setColor( const Color &color, size_t ind)
{
    pixels_[ind].color = sf::Color( color.r, color.g, color.b, color.a);
}


Color PixelsArray::getColor( size_t ind) const
{
    sf::Color color = pixels_[ind].color;

    return Color( color.r, color.g, color.b, color.a);
}


void PixelsArray::setPosition(const vec2i &coord, size_t ind)
{
    pixels_[ind].position = sf::Vector2f( static_cast<float>( coord.x), static_cast<float>( coord.y));
}


void PixelsArray::setPosition(const vec2f &coord, size_t ind)
{
    pixels_[ind].position = sf::Vector2f( coord.x, coord.y);
}


void PixelsArray::setPosition(const vec2d &coord, size_t ind)
{
    pixels_[ind].position = sf::Vector2f( static_cast<float>( coord.x), static_cast<float>( coord.y));
}


void PixelsArray::setPosition(int x, int y, size_t ind)
{
    pixels_[ind].position = sf::Vector2f( static_cast<float>( x), static_cast<float>( y));
}


void PixelsArray::setPosition(float x, float y, size_t ind)
{
    pixels_[ind].position = sf::Vector2f( x, y);
}


void PixelsArray::setPosition(double x, double y, size_t ind)
{
    pixels_[ind].position = sf::Vector2f( static_cast<float>( x), static_cast<float>( y));
}


void PixelsArray::draw( IRenderWindow *renderWindow) const
{
    assert( renderWindow );

    RenderWindow *w_ptr = dynamic_cast<RenderWindow *>( renderWindow);
    assert( w_ptr && "Failed to cast IRenderWindow to RenderWindow" );

    w_ptr->getWindow().draw( pixels_);
}


void Image::create(unsigned int width, unsigned int height, const Color &color /*= Color(0, 0, 0)*/)
{
    image_.create( width, height, sf::Color( color.r, color.g, color.b, color.a));
}


void Image::create(vec2u size, const Color &color /*= Color(0, 0, 0)*/)
{
    create( size.x, size.y, color);
}


void Image::create(unsigned int width, unsigned int height, const Color *pixels)
{
    assert( 0 && "Not implemented" );
}


void Image::create(vec2u size, const Color *pixels)
{
    create( size.x, size.y, pixels);
}


bool Image::loadFromFile(const std::string &filename)
{
    return image_.loadFromFile( filename);
}


vec2u Image::getSize() const
{
    return vec2u( image_.getSize().x, image_.getSize().y);
}


void Image::setPixel(unsigned int x, unsigned int y, const Color &color)
{
    image_.setPixel( x, y, sf::Color( color.r, color.g, color.b, color.a));
}


void Image::setPixel(vec2u pos, const Color &color)
{
    setPixel( pos.x, pos.y, color);
}


Color Image::getPixel(unsigned int x, unsigned int y) const
{
    sf::Color color = image_.getPixel( x, y);

    return Color( color.r, color.g, color.b, color.a);
}


Color Image::getPixel(vec2u pos) const
{
    return getPixel( pos.x, pos.y);
}


bool Texture::create(unsigned int width, unsigned int height)
{
    return texture_.create( width, height);
}


bool Texture::loadFromFile(const std::string& filename, const IntRect& area /*= IntRect()*/)
{
    sf::IntRect sf_area( area.top_x, area.top_y, area.width, area.height);
    return texture_.loadFromFile( filename, sf_area);
}


bool Texture::loadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect())
{
    sf::IntRect sf_area( area.top_x, area.top_y, area.width, area.height);
    return texture_.loadFromMemory( data, size, sf_area);
}


vec2u Texture::getSize() const
{
    return vec2u( texture_.getSize().x, texture_.getSize().y);
}


std::unique_ptr<IImage> Texture::copyToImage() const
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


void Texture::update(const IImage *image)
{
    assert( 0 && "Not implemented" );
}


void Texture::update(const Color *pixels)
{
    assert( 0 && "Not implemented" );
}


void Texture::update(const Color *pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
    assert( 0 && "Not implemented" );
}


void Sprite::setTexture(const ITexture *texture, bool reset_rect /*= false*/)
{
    assert( 0 && "Not implemented" );
}


void Sprite::setTextureRect(const IntRect &rectangle)
{
    sf::IntRect sf_area( rectangle.top_x, rectangle.top_y, rectangle.width, rectangle.height);
    sprite_.setTextureRect( sf_area);
}


void Sprite::setPosition(float x, float y)
{
    sprite_.setPosition( x, y);
}


void Sprite::setPosition(const vec2f &pos)
{
    sprite_.setPosition( pos.x, pos.y);
}


void Sprite::setScale(float factorX, float factorY)
{
    sprite_.setScale( factorX, factorY);
}


vec2u Sprite::getSize() const
{
    return vec2u( sprite_.getTextureRect().width, sprite_.getTextureRect().height);
}


void Sprite::setColor(const Color &color)
{
    sprite_.setColor( sf::Color( color.r, color.g, color.b, color.a));
}


void Sprite::setRotation(float angle)
{
    sprite_.setRotation( angle);
}


const vec2f Sprite::getPosition() const
{
    return vec2f( sprite_.getPosition().x, sprite_.getPosition().y);
}


IntRect Sprite::getGlobalBounds() const
{
    sf::IntRect rect = sprite_.getTextureRect();
    return IntRect( rect.left, rect.top, rect.width, rect.height);
}


bool Font::loadFromFile( const std::string &filename)
{
    return font_.loadFromFile( filename);
}


void Text::setString(const std::string& string)
{
    text_.setString( string);
}


void Text::setFont(const IFont* font)
{
    assert( 0 && "Not implemented" );
}


void Text::setCharacterSize(unsigned int size)
{
    text_.setCharacterSize( size);
}


void Text::setStyle(uint32_t style)
{
    text_.setStyle( style);
}


void Text::setFillColor(const Color* color)
{
    assert( color );

    text_.setFillColor( sf::Color( color->r, color->g, color->b, color->a));
}


void Text::setOutlineColor(const Color* color)
{
    assert( color );

    text_.setOutlineColor( sf::Color( color->r, color->g, color->b, color->a));
}


void Text::setOutlineThickness(float thickness)
{
    text_.setOutlineThickness( thickness);
}


void RectangleShape::setTexture(const ITexture *texture)
{
    assert( 0 && "Not implemented");
}


void RectangleShape::setFillColor(const Color &color)
{
    assert( 0 && "Not implemented");
}


void RectangleShape::setPosition(const vec2i &pos)
{
    assert( 0 && "Not implemented");
}


void RectangleShape::setPosition(const vec2f &pos)
{
    assert( 0 && "Not implemented");
}


void RectangleShape::setPosition(const vec2d &pos)
{
    assert( 0 && "Not implemented");
}


void RectangleShape::setScale(const vec2f &scale)
{
    assert( 0 && "Not implemented");
}


void RectangleShape::setSize(const vec2u &size)
{
    assert( 0 && "Not implemented");
}


void RectangleShape::setRotation(float angle)
{
    assert( 0 && "Not implemented");
}


void RectangleShape::setOutlineColor(const Color &color)
{
    assert( 0 && "Not implemented");
}


void RectangleShape::setOutlineThickness(float thickness)
{
    assert( 0 && "Not implemented");
}


float RectangleShape::getRotation() const
{
    assert( 0 && "Not implemented");

    return 0;
}


vec2f RectangleShape::getScale() const
{
    assert( 0 && "Not implemented");

    return vec2f();
}


vec2f RectangleShape::getPosition() const
{
    assert( 0 && "Not implemented");

    return vec2f();
}


const Color &RectangleShape::getFillColor() const
{
    assert( 0 && "Not implemented" );

    return *new Color();
}


vec2u RectangleShape::getSize() const
{
    assert( 0 && "Not implemented");

    return vec2u();
}


float RectangleShape::getOutlineThickness() const
{
    assert( 0 && "Not implemented");

    return 0;
}


const Color &RectangleShape::getOutlineColor() const
{
    assert( 0 && "Not implemented");

    return *new Color();
}


const IImage *RectangleShape::getImage() const
{
    assert( 0 && "Not implemented");

    return nullptr;
}


void RectangleShape::move(const vec2f &offset)
{
    assert( 0 && "Not implemented");
}


void EllipseShape::setTexture(const ITexture *texture)
{
    assert( 0 && "Not implemented");
}


void EllipseShape::setFillColor(const Color &color)
{
    assert( 0 && "Not implemented");
}


void EllipseShape::setPosition(const vec2i &pos)
{
    assert( 0 && "Not implemented");
}


void EllipseShape::setPosition(const vec2f &pos)
{
    assert( 0 && "Not implemented");
}


void EllipseShape::setPosition(const vec2d &pos)
{
    assert( 0 && "Not implemented");
}


void EllipseShape::setScale(const vec2f &scale)
{
    assert( 0 && "Not implemented");
}


void EllipseShape::setSize(const vec2u &size)
{
    assert( 0 && "Not implemented");
}


void EllipseShape::setRotation(float angle)
{
    assert( 0 && "Not implemented");
}


void EllipseShape::setOutlineColor(const Color &color)
{
    assert( 0 && "Not implemented");
}


void EllipseShape::setOutlineThickness(float thickness)
{
    assert( 0 && "Not implemented");
}


float EllipseShape::getRotation() const
{
    assert( 0 && "Not implemented");

    return 0;
}


vec2f EllipseShape::getScale() const
{
    assert( 0 && "Not implemented");

    return vec2f();
}


vec2f EllipseShape::getPosition() const
{
    assert( 0 && "Not implemented");

    return vec2f();
}


const Color &EllipseShape::getFillColor() const
{
    assert( 0 && "Not implemented" );

    return *new Color();
}


vec2u EllipseShape::getSize() const
{
    assert( 0 && "Not implemented");

    return vec2u();
}


float EllipseShape::getOutlineThickness() const
{
    assert( 0 && "Not implemented");

    return 0;
}


const Color &EllipseShape::getOutlineColor() const
{
    assert( 0 && "Not implemented");

    return *new Color();
}


const IImage *RectangleShape::getImage() const
{
    assert( 0 && "Not implemented");

    return nullptr;
}


void RectangleShape::move(const vec2f &offset)
{
    assert( 0 && "Not implemented");
}


} // namespace sfm


} // namespace psapi