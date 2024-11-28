#ifndef PHOTOSHOP_WINDOWS_IMPLEMENTATION
#define PHOTOSHOP_WINDOWS_IMPLEMENTATION


#include <api_photoshop.hpp>
#include <list>
#include <api_impl/sfm.hpp>
#include <SFML/Graphics.hpp>


using namespace psapi;


class AWindow : public IWindow
{
protected:
    bool is_active_;
    vec2i pos_;
    vec2u size_;
    wid_t id_;

    const IWindow *parent_;

public:
    virtual ~AWindow() = default;

    virtual wid_t getId() const override;
    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;

    virtual void setParent(const IWindow* parent) override;
    virtual void setSize(const vec2u &size) override;
    virtual void setPos(const vec2i &pos) override;

    virtual bool isActive() const override;
    virtual void forceActivate() override;
    virtual void forceDeactivate() override;

    virtual IWindow *getWindowById( wid_t id) override;
    virtual const IWindow *getWindowById( wid_t id) const override;
};


class AWindowContainer : public IWindowContainer
{
protected:
    std::list<std::unique_ptr<IWindow>> windows_;
    virtual void drawChildren( IRenderWindow *renderWindow) = 0;
public:
    virtual ~AWindowContainer() = default;

    virtual void draw(IRenderWindow *renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;

    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow *getWindowById(wid_t id) const override;

    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(wid_t id) override;
    virtual bool isWindowContainer() const override;
};


class RootWindow : public AWindowContainer
{
    const unsigned int kRootWindowWidth  = 1600;
    const unsigned int kRootWindowHeight = 900;
    const std::string kRootWindowTitle = "PhotoShop";

    sfm::RenderWindow window_;
    bool is_active_ = true;
    vec2u size_ = vec2u(kRootWindowWidth, kRootWindowHeight);
    vec2i pos_ = vec2i(0, 0);

    RootWindow();

    RootWindow( RootWindow &window) = delete;
    RootWindow &operator=( const RootWindow &window) = delete;

public:

    IRenderWindow *getRenderWindow();
    const IRenderWindow *getRenderWindow() const;

    static IWindowContainer *getInstance();

    IWindow *getWindowById( wid_t id) override;
    const IWindow *getWindowById( wid_t id) const override;

    wid_t getId() const override;
    vec2u getSize() const override;
    vec2i getPos() const override;

    void setParent(const IWindow *parent) override;
    void setSize(const vec2u &size) override;
    void setPos(const vec2i &pos) override;

    void forceActivate() override;
    void forceDeactivate() override;

    bool isActive() const override;
    void drawChildren( IRenderWindow *renderWindow) override;

};


#endif // PHOTOSHOP_WINDOWS_IMPLEMENTATION
