#include "filters.hpp"
#include <cassert>
#include <iostream>
#include "api_impl/canvas.hpp"
#include "fill_menu.hpp"
#include "windows_id.hpp"


const std::array<float, 9> GAUSS_MATRIX =
{
    0.025, 0.1, 0.025,
    0.1, 0.5, 0.1,
    0.025, 0.1, 0.025
};
const float GAUSS_SUM = 1;


BareliefFilter::BareliefFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                               std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> BareliefFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<BareliefFilterAction>(this, renderWindow, event);
}


void BareliefFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


BareliefFilterAction::BareliefFilterAction(BareliefFilter *button, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool BareliefFilterAction::isUndoable(const Key &key)
{
    return false;
}


bool BareliefFilterAction::undo(const Key &key)
{
    std::cout << "Undo filter\n";
    return true;
}


bool BareliefFilterAction::redo(const Key &key)
{
    std::cout << "Redo filter\n";
    return true;
}


bool BareliefFilterAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);

    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }

    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );
    canvas_->insertEmptyLayer( 1);
    Layer *new_layer = static_cast<Layer *>(canvas_->getLayer( 1));
    assert( new_layer );
    assert( new_layer != layer );

    sfm::vec2u canvas_size = canvas_->getSize();
    for ( unsigned x = 0; x < canvas_size.x; x++ )
    {
        for ( unsigned y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color_1 = layer->getPixelGlobal( sfm::vec2i( std::min( x + 4, canvas_size.x - 1), y));
            sfm::Color color_2 = layer->getPixelGlobal( sfm::vec2i( x, std::min( y + 4, canvas_size.y - 1)));

            sfm::Color color;
            color.r = (255 - color_1.r + color_2.r) / 2;
            color.g = (255 - color_1.g + color_2.g) / 2;
            color.b = (255 - color_1.b + color_2.b) / 2;
            color.a = (255 - color_1.a + color_2.a) / 2;
            new_layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }

    canvas_->removeLayer( 0);
    button_->setState( IBarButton::State::Normal);

    return true;
}



BlurFilter::BlurFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
            std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> BlurFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<BlurFilterAction>(this, renderWindow, event);
}


void BlurFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


BlurFilterAction::BlurFilterAction(BlurFilter* filter, const IRenderWindow* renderWindow, const Event& event)
    :   AUndoableAction(renderWindow, event), button_(filter)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool BlurFilterAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);

    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }
    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
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
                    sfm::Color color = layer->getPixelGlobal( sfm::vec2i( i, j));
                    r += color.r;
                    g += color.g;
                    b += color.b;
                    a += color.a;
                }
            }
            r /= 9; g /= 9; b /= 9; a /= 9;
            sfm::Color color( r, g, b, a);
            layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    button_->setState( IBarButton::State::Normal);

    return true;
}


bool BlurFilterAction::undo(const Key& key)
{
    std::cout << "Undo blur\n";

    return true;
}


bool BlurFilterAction::redo(const Key& key)
{
    std::cout << "Redo blur\n";
    return true;
}


bool BlurFilterAction::isUndoable(const Key& key)
{
    return false;
}


GaussBlurFilter::GaussBlurFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                                 std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> GaussBlurFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<GaussBlurFilterAction>(this, renderWindow, event);
}


void GaussBlurFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


GaussBlurFilterAction::GaussBlurFilterAction(GaussBlurFilter* filter, const IRenderWindow* renderWindow, const Event& event)
    :   AUndoableAction(renderWindow, event), button_(filter)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool GaussBlurFilterAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);
    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }

    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );
    assert( canvas_->insertEmptyLayer( 1) );
    Layer *new_layer = static_cast<Layer *>(canvas_->getLayer( 1));
    assert( new_layer );

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
                    sfm::Color color = layer->getPixelGlobal( sfm::vec2i( i, j));
                    r += color.r * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    g += color.g * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    b += color.b * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    a += color.a * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                }
            }
            r /= GAUSS_SUM; g /= GAUSS_SUM; b /= GAUSS_SUM; a /= GAUSS_SUM;
            sfm::Color color( static_cast<uint8_t>( r), static_cast<uint8_t>( g), static_cast<uint8_t>( b), static_cast<uint8_t>( a));
            new_layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    canvas_->removeLayer( 0);
    button_->setState( IBarButton::State::Normal);

    return true;
}


bool GaussBlurFilterAction::undo(const Key& key)
{
    std::cout << "Undo gauss blur\n";
    return true;
}


bool GaussBlurFilterAction::redo(const Key& key)
{
    std::cout << "Redo gauss blur\n";
    return true;
}


bool GaussBlurFilterAction::isUndoable(const Key& key)
{
    return false;
}


NegativeFilter::NegativeFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                               std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


void NegativeFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


std::unique_ptr<IAction> NegativeFilter::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<NegativeFilterAction>(this, renderWindow, event);
}


NegativeFilterAction::NegativeFilterAction(NegativeFilter *button, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool NegativeFilterAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);
    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }

    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );

    sfm::vec2u canvas_size = canvas_->getSize();
    for ( size_t x = 0; x < canvas_size.x; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color = layer->getPixelGlobal( sfm::vec2i( x, y));
            color.r = 255 - color.r;
            color.g = 255 - color.g;
            color.b = 255 - color.b;
            layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    button_->setState( IBarButton::State::Normal);

    return true;
}


bool NegativeFilterAction::undo(const Key& key)
{
    std::cout << "Undo negative filter\n";
    return true;
}


bool NegativeFilterAction::redo(const Key& key)
{
    std::cout << "Redo negative filter\n";
    return true;
}


bool NegativeFilterAction::isUndoable(const Key& key)
{
    return false;
}


BrightnessFilter::BrightnessFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                                   std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


void BrightnessFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


std::unique_ptr<IAction> BrightnessFilter::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<BrightnessFilterAction>(this, renderWindow, event);
}


BrightnessFilterAction::BrightnessFilterAction(BrightnessFilter *button, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool BrightnessFilterAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);
    int delta = -10;
    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }
    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );

    sfm::vec2u canvas_size = canvas_->getSize();
    for ( size_t x = 0; x < canvas_size.x; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color = layer->getPixelGlobal( sfm::vec2i( x, y));
            color.a += delta;
            layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    button_->setState( IBarButton::State::Normal);

    return true;
}


bool BrightnessFilterAction::undo(const Key& key)
{
    std::cout << "Undo brightness filter\n";
    return true;
}


bool BrightnessFilterAction::redo(const Key& key)
{
    std::cout << "Redo brightness filter\n";
    return true;
}


bool BrightnessFilterAction::isUndoable(const Key& key)
{
    return false;
}


ControlPanel::ControlPanel()
{
    std::unique_ptr<IWindow> button_ok_tmp = createMenuButton<TextButton>(kOkButtonId, "Ok");
    std::unique_ptr<IWindow> button_cancel_tmp = createMenuButton<TextButton>(kCancelButtonId, "Cancel");
    button_ok_ = std::unique_ptr<TextButton>(static_cast<TextButton *>(button_ok_tmp.release()));
    button_cancel_ = std::unique_ptr<TextButton>(static_cast<TextButton *>(button_cancel_tmp.release()));

    button_ok_->setSize(buttons_rect_size_);
    button_cancel_->setSize(buttons_rect_size_);
    min_buttons_width_ = std::max(button_ok_->getSize().x, button_cancel_->getSize().x) * 2;
    size_.x = min_buttons_width_;

    button_ok_->setPos(vec2i(0, 50));
    button_cancel_->setPos(vec2i(button_ok_->getSize().x, 50));

    shape_->setFillColor(sfm::Color::getStandardColor(psapi::sfm::Color::Type::White));
    shape_->setOutlineColor(sfm::Color::getStandardColor(psapi::sfm::Color::Type::Black));
    shape_->setOutlineThickness(5);
    shape_->setSize(size_);
    shape_->setPosition(vec2i(0, 0));

    normal_->setFillColor(sfm::Color());
    normal_->setOutlineColor(sfm::Color());
    normal_->setOutlineThickness(5);
    onHover_->setFillColor(sfm::Color());
    onHover_->setOutlineColor(sfm::Color(192, 192, 192));
    onHover_->setOutlineThickness(5);
    pressed_->setFillColor(sfm::Color());
    pressed_->setOutlineColor(sfm::Color::getStandardColor(psapi::sfm::Color::Type::Black));
    pressed_->setOutlineThickness(5);
    released_->setFillColor(sfm::Color());
    released_->setOutlineColor(sfm::Color());
    released_->setOutlineThickness(5);
}


void ControlPanel::draw(IRenderWindow* renderWindow)
{
    if ( !active_ )
        return;
    shape_->draw(renderWindow);
    button_ok_->draw(renderWindow);
    button_cancel_->draw(renderWindow);
    finishButtonDraw(renderWindow, button_ok_.get());
    finishButtonDraw(renderWindow, button_cancel_.get());
}


void ControlPanel::finishButtonDraw(IRenderWindow* renderWindow, const TextButton* button)
{
    vec2i pos = button->getPos();
    vec2u size = button->getSize();
    switch ( button->getState() )
    {
        case IBarButton::State::Normal:
            normal_->setPosition( pos);
            normal_->setSize(size);
            renderWindow->draw( normal_.get());
            break;
        case IBarButton::State::Hover:
            onHover_->setPosition( pos);
            onHover_->setSize(size);
            renderWindow->draw( onHover_.get());
            break;
        case IBarButton::State::Press:
            pressed_->setPosition( pos);
            pressed_->setSize(size);
            renderWindow->draw( pressed_.get());
            break;
        case IBarButton::State::Released:
            released_->setPosition( pos);
            released_->setSize(size);
            renderWindow->draw( released_.get());
            break;
        default:
            assert( 0 && "Unreachable" );

    }
}


std::unique_ptr<IAction> ControlPanel::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<ControlPanelAction>(this, renderWindow, event);
}


wid_t ControlPanel::getId() const
{
    return kControlPanelId;
}


IWindow* ControlPanel::getWindowById(wid_t id)
{
    if ( id == kControlPanelId )
        return this;
    if ( id == kOkButtonId )
        return button_ok_.get();
    if ( id == kCancelButtonId )
        return button_cancel_.get();
    return nullptr;
}


const IWindow* ControlPanel::getWindowById(wid_t id) const
{
    if ( id == kControlPanelId )
        return this;
    if ( id == kOkButtonId )
        return button_ok_.get();
    if ( id == kCancelButtonId )
        return button_cancel_.get();
    return nullptr;
}


vec2i ControlPanel::getPos() const
{
    return pos_;
}


vec2u ControlPanel::getSize() const
{
    return size_;
}


void ControlPanel::setSize(const vec2u& size)
{
    int buttons_width = std::max(min_buttons_width_, size.x);
    int buttons_height = std::max(buttons_rect_size_.y, size.y);

    vec2i button_ok_pos = button_ok_->getPos();
    vec2i button_cancel_pos = button_cancel_->getPos();
    vec2u button_ok_size = button_ok_->getSize();
    vec2u button_cancel_size = button_cancel_->getSize();

    int button_ok_offset = (buttons_width / 2 - button_ok_size.x) / 2;
    int button_cancel_offset = (buttons_width / 2 - button_cancel_size.x) / 2;

    button_ok_pos.x = button_ok_offset;
    button_ok_pos.y = buttons_height - button_ok_size.y;
    button_cancel_pos.x = button_cancel_offset + buttons_width / 2;
    button_cancel_pos.y = buttons_height - button_cancel_size.y;

    button_ok_->setPos(button_ok_pos);
    button_cancel_->setPos(button_cancel_pos);

    size_ = vec2u(buttons_width, buttons_height);
    shape_->setSize(size_);

    setPos(getCanvasIntRect().pos + vec2i(getCanvasIntRect().size) - vec2i(size_) - vec2i(5, 5));
}


void ControlPanel::setPos(const vec2i& pos)
{
    vec2i diff = pos - pos_;
    pos_ = pos;
    button_ok_->setPos(button_ok_->getPos() + diff);
    button_cancel_->setPos(button_cancel_->getPos() + diff);
    shape_->setPosition(vec2i(shape_->getPosition().x, shape_->getPosition().y) + diff);
}


void ControlPanel::setParent(const IWindow* parent)
{
    assert( 0 && "I don't know who should be parent for this window" );
}


void ControlPanel::forceActivate()
{
    active_ = true;
}


void ControlPanel::forceDeactivate()
{
    active_ = false;
}


bool ControlPanel::isActive() const
{
    return active_;
}


bool ControlPanel::isWindowContainer() const
{
    return false;
}


int ControlPanel::getButtonsRectHeight() const
{
    return buttons_rect_size_.y;
}


vec2u ControlPanel::fitButtonsIn(vec2u size)
{
    unsigned int buttons_width = std::max(min_buttons_width_, size.x);
    unsigned int buttons_height = std::max<unsigned int>(50, size.y);

    vec2i button_ok_pos = button_ok_->getPos();
    vec2i button_cancel_pos = button_cancel_->getPos();
    vec2u button_ok_size = button_ok_->getSize();
    vec2u button_cancel_size = button_cancel_->getSize();

    int button_ok_offset = buttons_width / 2 - button_ok_size.x / 2;
    int button_cancel_offset = buttons_width / 2 - button_cancel_size.x / 2;

    button_ok_pos.x = button_ok_offset;
    button_ok_pos.y += buttons_height - size_.x;
    button_cancel_pos.x = button_cancel_offset + buttons_width / 2;
    button_cancel_pos.y += buttons_height - size_.x;

    button_ok_->setPos(button_ok_pos);
    button_cancel_->setPos(button_cancel_pos);

    return vec2u(buttons_width, buttons_height);
}


ControlPanelAction::ControlPanelAction(ControlPanel *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool ControlPanelAction::execute(const Key &key)
{
    getActionController()->execute(button_->button_ok_->createAction(render_window_, event_));
    if ( button_->button_ok_->getState() == psapi::IBarButton::State::Press )
    {
        button_->button_cancel_->setState(psapi::IBarButton::State::Normal);
    }
    getActionController()->execute(button_->button_cancel_->createAction(render_window_, event_));
    if ( button_->button_cancel_->getState() == psapi::IBarButton::State::Press )
    {
        button_->button_ok_->setState(psapi::IBarButton::State::Normal);
    }
    return true;
}


bool ControlPanelAction::isUndoable(const Key &key)
{
    return false;
}


ContrastControlPanel::ContrastControlPanel()
    :   ControlPanel()
{
    setSize(vec2u(600, 200));
    // main_shape_->setFillColor();
}


void ContrastControlPanel::setSize(const vec2u& size)
{
    ControlPanel::setSize(size);
    return;
}


void ContrastControlPanel::setPos(const vec2i& pos)
{
    ControlPanel::setPos(pos);
    return;
}


ABarButton::State ControlPanel::getOkState() const
{
    return button_ok_->getState();
}


ABarButton::State ControlPanel::getCancelState() const
{
    return button_cancel_->getState();
}


void ControlPanel::unsetAll()
{
    button_ok_->setState(psapi::IBarButton::State::Normal);
    button_cancel_->setState(psapi::IBarButton::State::Normal);
}


void ContrastControlPanel::draw(IRenderWindow* renderWindow)
{
    ControlPanel::draw(renderWindow);
    main_shape_->draw(renderWindow);
    line_shape_->draw(renderWindow);
    slider_shape_->draw(renderWindow);
}


void ContrastControlPanel::setFilter(ContrastFilter *parent)
{
    parent_ = parent;
}


void ContrastControlPanel::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(ControlPanel::createAction(renderWindow, event));
}


std::unique_ptr<IAction> ContrastControlPanel::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<ContrastControlPanelAction>(this, renderWindow, event);
}


ABarButton::State ContrastControlPanel::getOkState() const
{
    return ControlPanel::getOkState();
}


ABarButton::State ContrastControlPanel::getCancelState() const
{
    return ControlPanel::getCancelState();
}


ContrastControlPanelAction::ContrastControlPanelAction(ContrastControlPanel *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool ContrastControlPanelAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    if ( button_->getOkState() == IBarButton::State::Press )
    {
        button_->parent_->setOk(true);
        button_->parent_->setCancel(false);
    } else if ( button_->getCancelState() == IBarButton::State::Press )
    {
        button_->parent_->setCancel(true);
        button_->parent_->setOk(false);
    }
    return true;
}


bool ContrastControlPanelAction::isUndoable(const Key &key)
{
    return false;
}
