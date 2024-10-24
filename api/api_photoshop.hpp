/**
 * @file api_photoshop.hpp
 * @brief API for Photoshop
 */

#ifndef API_PHOTOSHOP_HPP
#define API_PHOTOSHOP_HPP

#include <cstdint>
#include <memory>
#include <vector>

namespace psapi {

namespace sfm {
    class ARenderWindow;
    class Event;
} // namespace sfm

using sfm::ARenderWindow;
using sfm::Event;

/**
 * @brief Alias for window identifier type.
 */
using wid_t = int64_t;

/**
 * @brief Represents a 2D integer vector (position or size).
 */
struct vec2i
{
    int x; ///< X-coordinate
    int y; ///< Y-coordinate
};

/** @brief Invalid window ID constant. */
const wid_t kInvalidWindowId = -1;

/**
 * @brief Interface representing a basic window.
 */
class IWindow
{
public:
    /**
     * @brief Renders the window.
     * @param renderWindow The render target.
     */
    virtual void draw(ARenderWindow* renderWindow) = 0;

    /**
     * @brief Updates the window based on events.
     * @param renderWindow The render target.
     * @param event The event to process.
     * @return True if the window was updated, false otherwise.
     */
    virtual bool update(const ARenderWindow* renderWindow, const Event& event) = 0;

    /**
     * @brief Gets the unique ID of the window.
     * @return The window ID, or kInvalidWindowId if not set.
     */
    virtual int64_t getId() const { return kInvalidWindowId; }

    /**
     * @brief Retrieves a window by its ID from this window or its children.
     * @param id The ID of the window to search for.
     * @return A pointer to the window if found, or nullptr if not found.
     */
    virtual IWindow* getWindowById(wid_t id) = 0;

    /**
     * @brief Retrieves a window by its ID from this window or its children (const version).
     * @param id The ID of the window to search for.
     * @return A const pointer to the window if found, or nullptr if not found.
     */
    virtual const IWindow* getWindowById(wid_t id) const = 0;

    /**
     * @brief Gets the position of the window.
     * @return The absolute window position as a vec2i.
     */
    virtual vec2i getPos() const = 0;

    /**
     * @brief Gets the size of the window.
     * @return The window size as a vec2i.
     */
    virtual vec2i getSize() const = 0;

    /**
     * @brief Sets the parent of this window.
     * @param parent A pointer to the parent window.
     */
    virtual void setParent(const IWindow* parent) = 0;

    /**
     * @brief Forces the window to activate.
     */
    virtual void forceActivate() = 0;
    
    /**
     * @brief Forces the window to deactivate.
     */
    virtual void forceDeactivate() = 0;
};

/**
 * @brief Interface representing a container of windows.
 */
class IWindowContainer : public IWindow
{
public:
    /**
     * @brief Adds a new window to the container.
     * @param window The window to add.
     */
    virtual void addWindow(std::unique_ptr<IWindow> window) = 0;

    /**
     * @brief Removes a window from the container by its ID.
     * @param id The ID of the window to remove.
     */
    virtual void removeWindow(wid_t id) = 0;
};

/**
 * @brief A container class that stores windows in a vector.
 */
class IWindowVector : public IWindowContainer
{
public:
    /**
     * @brief Adds a new window to the vector container.
     * @param window The window to add.
     */
    virtual void addWindow(std::unique_ptr<IWindow> window) override;

    /**
     * @brief Removes a window from the vector container by its ID.
     * @param id The ID of the window to remove.
     */
    virtual void removeWindow(wid_t id) override;

protected:
    std::vector<std::unique_ptr<IWindow> > windows_; ///< Vector of windows.
};

/**
 * @brief Retrieves the root window of the application.
 * @return A pointer to the root window.
 */
IWindow* getRootWindow();

/**
 * @brief Function pointer type for general-purpose functions.
 */
using generalFunction = void* (*)(void*);

/**
 * @brief Retrieves a general-purpose function by name.
 * @param name The name of the function.
 * @return A pointer to the function, or nullptr if not found.
 */
generalFunction getGeneralFunction(const std::string& name);

} // namespace psapi

#endif // API_PHOTOSHOP_HPP

