#ifndef PHOTOSHOP_BAR_IMPLEMENTATION
#define PHOTOSHOP_BAR_IMPLEMENTATION


#include <api_bar.hpp>


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


class ABar : public IBar
{
public:
    virtual ~ABar() = default;

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

    virtual ChildInfo getNextChildInfo() const override;
    virtual void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const override;
};


#endif // PHOTOSHOP_BAR_IMPLEMENTATION
