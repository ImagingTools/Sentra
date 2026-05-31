// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Qt includes
#include <QtCore/QString>
#include <QtCore/QList>

// Standard includes
#include <optional>

namespace imtsentra
{

/**
 * \brief Represents a screen/page in the application map
 */
struct AppScreen {
    QString id;
    QString url;
    QString title;
    std::optional<QString> screenshotPath;
    QList<QString> elements;
    QString discoveredAt;
    QString lastSeenAt;
    int visitCount = 0;
};

/**
 * \brief Represents a transition between screens
 */
struct AppTransition {
    QString id;
    QString sourceScreenId;
    QString targetScreenId;
    QString action;
    std::optional<QString> label;
    QString discoveredAt;
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
        const QString& url,
        const QString& title,
        const std::optional<QString>& screenshotPath = std::nullopt
    ) = 0;

    /**
     * \brief Record a transition between screens
     * \param fromUrl Source screen URL
     * \param toUrl Target screen URL
     * \param action Description of the action that caused the transition
     */
    virtual void RecordTransition(
        const QString& fromUrl,
        const QString& toUrl,
        const QString& action
    ) = 0;

    /**
     * \brief Get all discovered screens
     */
    virtual QList<AppScreen> GetScreens() const = 0;

    /**
     * \brief Get all discovered transitions
     */
    virtual QList<AppTransition> GetTransitions() const = 0;

    /**
     * \brief Get screens that have no test scenarios covering them
     */
    virtual QList<AppScreen> GetUntestedScreens() const = 0;

    /**
     * \brief Calculate test coverage percentage
     */
    virtual float GetCoveragePercentage() const = 0;

    /**
     * \brief Export app map as JSON
     */
    virtual QString ToJson() const = 0;
};

typedef istd::TUniqueInterfacePtr<IAppMapBuilder> IAppMapBuilderUniquePtr;

} // namespace imtsentra
