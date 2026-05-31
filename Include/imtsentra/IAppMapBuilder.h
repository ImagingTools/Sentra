// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Standard includes
#include <string>
#include <vector>
#include <optional>

namespace imtsentra
{

/**
 * \brief Represents a screen/page in the application map
 */
struct AppScreen {
    std::string id;
    std::string url;
    std::string title;
    std::optional<std::string> screenshotPath;
    std::vector<std::string> elements;
    std::string discoveredAt;
    std::string lastSeenAt;
    int visitCount = 0;
};

/**
 * \brief Represents a transition between screens
 */
struct AppTransition {
    std::string id;
    std::string sourceScreenId;
    std::string targetScreenId;
    std::string action;
    std::optional<std::string> label;
    std::string discoveredAt;
};

/**
 * \brief Interface for building and managing the application map
 *
 * Automatically builds a graph of application screens and transitions
 * by tracking navigation during test executions.
 *
 * \ingroup imtsentra
 */
class IAppMapBuilder: virtual public istd::IPolymorphic
{
public:
    /**
     * \brief Record a screen visit during execution
     * \param url Current URL
     * \param title Page title
     * \param screenshotPath Optional screenshot of the screen
     */
    virtual void RecordScreen(
        const std::string& url,
        const std::string& title,
        const std::optional<std::string>& screenshotPath = std::nullopt
    ) = 0;

    /**
     * \brief Record a transition between screens
     * \param fromUrl Source screen URL
     * \param toUrl Target screen URL
     * \param action Description of the action that caused the transition
     */
    virtual void RecordTransition(
        const std::string& fromUrl,
        const std::string& toUrl,
        const std::string& action
    ) = 0;

    /**
     * \brief Get all discovered screens
     */
    virtual std::vector<AppScreen> GetScreens() const = 0;

    /**
     * \brief Get all discovered transitions
     */
    virtual std::vector<AppTransition> GetTransitions() const = 0;

    /**
     * \brief Get screens that have no test scenarios covering them
     */
    virtual std::vector<AppScreen> GetUntestedScreens() const = 0;

    /**
     * \brief Calculate test coverage percentage
     */
    virtual float GetCoveragePercentage() const = 0;

    /**
     * \brief Export app map as JSON
     */
    virtual std::string ToJson() const = 0;
};

typedef istd::TUniqueInterfacePtr<IAppMapBuilder> IAppMapBuilderUniquePtr;

} // namespace imtsentra
