#include "brightness.hpp"
#include <iostream>
#include <cassert>
#include "api_impl/memento.hpp"


BrightnessFilter::BrightnessFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                                   std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)), panel_(std::make_unique<BrightnessFilterControlPanel>()),
        memento_(std::make_unique<AMementable<ILayerSnapshot>>())
{
    panel_->setFilter(this);
}


void BrightnessFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


std::unique_ptr<IAction> BrightnessFilter::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<BrightnessFilterAction>(this, renderWindow, event);
}


void BrightnessFilter::draw(IRenderWindow *render_window)
{
    TextButton::draw(render_window);
    if  ( panel_->isActive() )
    {
        panel_->draw(render_window);
    }
}


void BrightnessFilterControlPanel::drawPoint(sfm::vec2i pos, sfm::Color color, int radius)
{
    int radius2 = radius * radius;

    for ( int x = -radius; x < radius; x++ )
    {
        for ( int y = -radius; y < radius; y++ )
        {
            if ( x * x + y * y <= radius2 )
            {
                image_->setPixel( pos.x, pos.y, color);
            }
            pos.y++;
        }
        pos.y -= 2 * radius;
        pos.x++;
    }
}


void BrightnessFilter::setOk(bool value)
{
    is_ok_ = value;
}


void BrightnessFilter::setApplied(bool value)
{
    is_applied_ = value;
}


void BrightnessFilter::setCancel(bool value)
{
    is_cancel_ = value;
}


void BrightnessFilter::unsetAll()
{
    panel_->unsetAll();
}


void BrightnessFilter::updatePanel(const IRenderWindow *render_window, const Event &event)
{
    getActionController()->execute(panel_->createAction(render_window, event));
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
    if ( button_->getState() != IBarButton::State::Press )
    {
        if ( button_->getState() != psapi::IBarButton::State::Hover )
        {
            button_->panel_->forceDeactivate();
        }
        return true;
    }
    button_->panel_->forceActivate();
    button_->updatePanel(render_window_, event_);
    if ( !button_->is_applied_ )
    {
        applyToTmpLayer();
        button_->is_applied_ = true;
    }
    if ( button_->is_ok_)
    {
        applyToMainLayer();
    }
    else if ( button_->is_cancel_ )
    {
        removeTmpLayer();
    } else
    {
        return true;
    }
    button_->unsetAll();
    button_->setState(psapi::IBarButton::State::Normal);
    button_->is_applied_ = false;
    button_->is_ok_ = false;
    button_->is_cancel_ = false;

    return true;
}


void BrightnessFilterAction::applyToTmpLayer()
{
    main_layer_ = static_cast<ILayer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    button_->snapshot_ = button_->memento_->save();

    const std::vector<std::pair<int, int>> &points = button_->panel_->getBrightnessPoints();
    sfm::IntRect rect = getCanvasIntRect();
    for ( size_t x = 0; x < rect.size.x; x++ )
    {
        for ( size_t y = 0; y < rect.size.y; y++ )
        {
            sfm::Color color = main_layer_->getPixel( sfm::vec2i( x, y));
            color.a = static_cast<uint8_t>(255 - points[x].second);
            main_layer_->setPixel( sfm::vec2i( x, y), color);
        }
    }
}


void BrightnessFilterAction::applyToMainLayer()
{

}


void BrightnessFilterAction::removeTmpLayer()
{
    button_->memento_->restore(button_->snapshot_.get());
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


BrightnessFilterControlPanel::BrightnessFilterControlPanel()
{
    setSize(vec2u(600, 335));
    frame_->setOutlineColor(sfm::Color( 51, 153, 255));
    frame_->setOutlineThickness(5);
    interpolated_.resize(getCanvasIntRect().size.x);
    draw_rect_.pos = vec2i(frame_->getPosition().x, frame_->getPosition().y) + vec2i( 5, 5);
    draw_rect_.size = frame_->getSize() - vec2u(10, 18);
    image_->create(draw_rect_.size.x, draw_rect_.size.y, sfm::Color::getStandardColor(psapi::sfm::Color::Type::White));
    image_->setPos(draw_rect_.pos);
    texture_->create(draw_rect_.size.x, draw_rect_.size.y);
    sprite_->setTexture(texture_.get());

    points_.insert({0, 0});
    points_.insert({10, 20});
    points_.insert({150, 89});
    points_.insert({draw_rect_.size.x - 5, 0});
    is_added_ = true;
}


std::unique_ptr<IAction> BrightnessFilterControlPanel::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<BrightnessFilterControlPanelAction>(this, renderWindow, event);
}


void BrightnessFilterControlPanel::draw(IRenderWindow* renderWindow)
{
    ControlPanel::draw(renderWindow);
    frame_->draw(renderWindow);

    for ( auto &point : points_ )
    {
        drawPoint(vec2i(point.first, point.second), sfm::Color::getStandardColor(psapi::sfm::Color::Type::Blue), 4);
    }
    for ( auto &point : interpolated_ )
    {
        drawPoint(vec2i(point.first, point.second), sfm::Color::getStandardColor(psapi::sfm::Color::Type::Red), 2);
    }
    texture_->update(image_.get());
    sprite_->setTexture(texture_.get());
    sprite_->setPosition(draw_rect_.pos.x, draw_rect_.pos.y);
    sprite_->draw(renderWindow);
}


const std::vector<std::pair<int, int>> &BrightnessFilterControlPanel::getBrightnessPoints() const &
{
    return interpolated_;
}


void BrightnessFilterControlPanel::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(ControlPanel::createAction(renderWindow, event));
}


void BrightnessFilterControlPanel::setSize(const vec2u& size)
{
    ControlPanel::setSize(size);
    frame_->setSize(vec2u(size.x - 20, size.y - 50 - 20));
    frame_->setPosition(getPos() + vec2i(10, 10));
}


void BrightnessFilterControlPanel::setPos(const vec2i& pos)
{
    ControlPanel::setPos(pos);
    frame_->setPosition(getPos() + vec2i(10, 10));
}


void BrightnessFilterControlPanel::setFilter(BrightnessFilter *parent)
{
    parent_ = parent;
}


BrightnessFilterControlPanelAction::BrightnessFilterControlPanelAction(BrightnessFilterControlPanel *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button)
{}


bool BrightnessFilterControlPanelAction::execute(const Key& key)
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
    if ( button_->is_added_ )
    {
        interpolate();
        button_->is_added_ = false;
        button_->parent_->setApplied(false);
    }

    return true;
}


bool BrightnessFilterControlPanelAction::isUndoable(const Key &key)
{
    return false;
}


void BrightnessFilterControlPanelAction::interpolate()
{
     std::vector<std::pair<int, int>> &points = button_->interpolated_;
    std::set<std::pair<int, int>> &points_set = button_->points_;
    size_t size = points.size();
    size_t cnt_points = points_set.size();

    float steps_per_pixel = button_->draw_rect_.size.x / static_cast<float>(size);
    float step = 1.f / steps_per_pixel;

    for ( int i = 0; i < size; i++ )
    {
        points[i].first = i;
        points[i].second = rand() % 256;
    }

    auto it = points_set.begin();
    size_t index = 0;
    for ( size_t i = 0; i < cnt_points - 3; i++ )
    {
        auto old_it = it;
        vec2i p0 = vec2i(it->first, it->second);
        it++;
        vec2i p1 = vec2i(it->first, it->second);
        it++;
        vec2i p2 = vec2i(it->first, it->second);
        it++;
        vec2i p3 = vec2i(it->first, it->second);
        it = old_it;
        it++;
        for ( float t = 0; t < size; t += step )
        {
            points[index].first = index;
            points[index].second = rand() % 255;
            index++;
        }
    }
}


sfm::vec2i BrightnessFilterControlPanelAction::interpolatePoint( sfm::vec2i p0, sfm::vec2i p1, sfm::vec2i p2, sfm::vec2i p3, float t)
{
    double t2   = t  * t,
           t3   = t2 * t,
           t3_2 = t3 * 2,
           t2_3 = t2 * 3;

    double h00 =  t3_2 - t2_3 + 1;
    double h10 =  t3   - t2 * 2 + t;
    double h01 = -t3_2 + t2_3;
    double h11 =  t3   - t2;

    sfm::vec2f p0f = sfm::vec2f( p0.x, p0.y);
    sfm::vec2f p1f = sfm::vec2f( p1.x, p1.y);
    sfm::vec2f p2f = sfm::vec2f( p2.x, p2.y);
    sfm::vec2f p3f = sfm::vec2f( p3.x, p3.y);

    sfm::vec2f m0 = (p2f - p0f) * 0.5;
    sfm::vec2f m1 = (p3f - p1f) * 0.5;

    sfm::vec2f res = h00 * p1f + h10 * m0 + h01 * p2f + h11 * m1;
    return sfm::vec2i( std::round( res.x), std::round( res.y));
}
