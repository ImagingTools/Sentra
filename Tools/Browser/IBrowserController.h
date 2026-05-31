#pragma once

#include <string>
#include <vector>
#include <optional>
#include <functional>

namespace imtsentra {

/**
 * @brief Browser command types for the Playwright controller
 */
enum class BrowserCommand {
    Navigate,
    Click,
    Type,
    Screenshot,
    GetDom,
    GetAccessibilityTree,
    WaitFor,
    Evaluate,
    Select,
    Hover,
    Scroll
};

/**
 * @brief Request to send to the browser controller
 */
struct BrowserRequest {
    BrowserCommand command;
    std::string payload;  // JSON parameters
    int timeoutMs = 30000;
};

/**
 * @brief Response from the browser controller
 */
struct BrowserResponse {
    bool success;
    std::string data;      // JSON response data
    std::optional<std::string> error;
    int durationMs = 0;
};

/**
 * @brief Interface for browser controller communication
 *
 * Communicates with Playwright via WebSocket JSON-RPC protocol.
 * Provides navigation, interaction, screenshot capture, and DOM access.
 */
class IBrowserController {
public:
    virtual ~IBrowserController() = default;

    /**
     * @brief Connect to the Playwright controller
     * @param endpoint WebSocket endpoint URL
     * @return true if connection successful
     */
    virtual bool connect(const std::string& endpoint) = 0;

    /**
     * @brief Disconnect from the browser controller
     */
    virtual void disconnect() = 0;

    /**
     * @brief Check if connected
     */
    virtual bool isConnected() const = 0;

    /**
     * @brief Send a command to the browser
     */
    virtual BrowserResponse sendCommand(const BrowserRequest& request) = 0;

    // Convenience methods
    virtual BrowserResponse navigate(const std::string& url) = 0;
    virtual BrowserResponse click(const std::string& selector) = 0;
    virtual BrowserResponse type(const std::string& selector, const std::string& text) = 0;
    virtual BrowserResponse screenshot(bool fullPage = false) = 0;
    virtual BrowserResponse getDom() = 0;
    virtual BrowserResponse getAccessibilityTree() = 0;
    virtual BrowserResponse waitFor(const std::string& condition, int timeoutMs = 5000) = 0;

    /**
     * @brief Launch browser with given configuration
     * @param headless Whether to run in headless mode
     * @param viewportWidth Viewport width
     * @param viewportHeight Viewport height
     */
    virtual bool launch(bool headless = true, int viewportWidth = 1920, int viewportHeight = 1080) = 0;

    /**
     * @brief Close the browser
     */
    virtual void close() = 0;
};

} // namespace imtsentra
