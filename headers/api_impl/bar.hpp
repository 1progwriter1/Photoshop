#ifndef PHOTOSHOP_BAR_IMPLEMENTATION
#define PHOTOSHOP_BAR_IMPLEMENTATION


#include <api_bar.hpp>
#include <list>
#include <api_impl/sfm.hpp>


using namespace psapi;


class ABarButton : public IBarButton
{
public:
    virtual ~ABarButton() = default;

    virtual void draw(IRenderWindow* renderWindow) override;
    virtual bool update(const IRenderWindow* renderWindow, const Event& event) override;
    virtual wid_t getId() const override;
    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;
    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;
    virtual void setParent(const IWindow* parent) override;
    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    virtual void setState(State state) override;
    virtual State getState() const override;
};


class Bar : public IBar
{
    vec2u size_;
    vec2i pos_;

    bool is_active_;

    const IWindow *parent_;

    std::list<std::unique_ptr<IBarButton>> buttons_;

    psapi::sfm::RectangleShape shape_;

public:
    Bar( vec2u init_size, vec2i init_pos, const sfm::Color &in_color, const sfm::Color &out_color, const float thickness);
    ~Bar() = default;

    void draw(IRenderWindow* renderWindow) override;
    bool update(const IRenderWindow* renderWindow, const Event& event) override;
    wid_t getId() const override;
    IWindow* getWindowById(wid_t id) override;
    const IWindow* getWindowById(wid_t id) const override;
    vec2i getPos() const override;
    vec2u getSize() const override;
    void setParent(const IWindow* parent) override;
    void forceActivate() override;
    void forceDeactivate() override;
    bool isActive() const override;
    bool isWindowContainer() const override;
    void addWindow(std::unique_ptr<IWindow> window) override;
    void removeWindow(wid_t id) override;

    ChildInfo getNextChildInfo() const override;
    void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const override;
};


#endif // PHOTOSHOP_BAR_IMPLEMENTATION
