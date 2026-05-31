// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <string>
#include <vector>
#include <optional>
#include <functional>

namespace imtsentra
{

/**
 * \brief Browser command types for the Playwright controller
 */
enum BrowserCommand
{
    BC_NAVIGATE,
    BC_CLICK,
    BC_TYPE,
    BC_SCREENSHOT,
    BC_GET_DOM,
    BC_GET_ACCESSIBILITY_TREE,
    BC_WAIT_FOR,
    BC_EVALUATE,
    BC_SELECT,
    BC_HOVER,
    BC_SCROLL
};

/**
 * \brief Request to send to the browser controller
 */
struct BrowserRequest {
    BrowserCommand command;
    std::string payload;  // JSON parameters
    int timeoutMs = 30000;
};

/**
 * \brief Response from the browser controller
 */
struct BrowserResponse {
    bool success;
    std::string data;      // JSON response data
    std::optional<std::string> error;
    int durationMs = 0;
};

/**
 * \brief Interface for browser controller communication
 *
 * Communicates with Playwright via WebSocket JSON-RPC protocol.
 * Provides navigation, interaction, screenshot capture, and DOM access.
 */
class IBrowserController {
public:
    virtual ~IBrowserController() = default;

    /**
     * \brief Connect to the Playwright controller
     * \param endpoint WebSocket endpoint URL
     * \return true if connection successful
     */
    virtual bool Connect(const std::string& endpoint) = 0;

    /**
     * \brief Disconnect from the browser controller
     */
    virtual void Disconnect() = 0;

    /**
     * \brief Check if connected
     */
    virtual bool IsConnected() const = 0;

    /**
     * \brief Send a command to the browser
     */
    virtual BrowserResponse SendCommand(const BrowserRequest& request) = 0;

    // Convenience methods
    virtual BrowserResponse Navigate(const std::string& url) = 0;
    virtual BrowserResponse Click(const std::string& selector) = 0;
    virtual BrowserResponse Type(const std::string& selector, const std::string& text) = 0;
    virtual BrowserResponse Screenshot(bool fullPage = false) = 0;
    virtual BrowserResponse GetDom() = 0;
    virtual BrowserResponse GetAccessibilityTree() = 0;
    virtual BrowserResponse WaitFor(const std::string& condition, int timeoutMs = 5000) = 0;

    /**
     * \brief Launch browser with given configuration
     * \param headless Whether to run in headless mode
     * \param viewportWidth Viewport width
     * \param viewportHeight Viewport height
     */
    virtual bool Launch(bool headless = true, int viewportWidth = 1920, int viewportHeight = 1080) = 0;

    /**
     * \brief Close the browser
     */
    virtual void Close() = 0;
};

} // namespace imtsentra
