#ifndef PHOTOSHOP_WINDOWS_IMPLEMENTATION
#define PHOTOSHOP_WINDOWS_IMPLEMENTATION


#include <api_photoshop.hpp>
#include <list>
#include <vector>
#include <SFML/Graphics.hpp>


using namespace psapi;
using sfm::IRenderWindow;
using sfm::Event;


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
    virtual bool isActive() const override;
    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isWindowContainer() const override;

    virtual IWindow *getWindowById( wid_t id) override;
    virtual const IWindow *getWindowById( wid_t id) const override;
};


class AWindowContainer : public IWindowContainer
{
    std::list<std::unique_ptr<IWindow>> windows_;
protected:
    virtual void drawChildren( IRenderWindow *renderWindow);
public:
    ~AWindowContainer() = default;

    void draw( IRenderWindow *renderWindow) override;
    bool update( const IRenderWindow* renderWindow, const Event& event) override;

    IWindow* getWindowById( wid_t id) override;
    const IWindow *getWindowById( wid_t id) const override;

    void addWindow( std::unique_ptr<IWindow> window) override;
    void removeWindow( wid_t id) override;
    bool isWindowContainer() const override;
};


class AWindowVector : public IWindowVector
{
    std::vector<std::unique_ptr<IWindow>> windows_;
protected:
    virtual void drawChildren( IRenderWindow *renderWindow);
public:
    AWindowVector() = default;
    ~AWindowVector() = default;

    void draw( IRenderWindow *renderWindow) override;
    bool update( const IRenderWindow* renderWindow, const Event& event) override;

    void addWindow( std::unique_ptr<IWindow> window) override;
    void removeWindow( wid_t id) override;

          IWindow* getWindowById( wid_t id)       override;
    const IWindow* getWindowById( wid_t id) const override;
};


class RootWindow : public AWindowContainer
{
    sf::RenderWindow window_;
    unsigned int width_;
    unsigned int height_;
public:
    RootWindow( unsigned int init_width, unsigned int init_height, const std::string &title = "photoshop");
    ~RootWindow() = default;

    virtual void draw( IRenderWindow *renderWindow) override;
    virtual bool update( const IRenderWindow *renderWindow, const Event &event) override;
};


#endif // PHOTOSHOP_WINDOWS_IMPLEMENTATION
