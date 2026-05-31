// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <istd/istd.h>
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Qt includes
#include <QtCore/QString>
#include <QtCore/QList>

// Standard includes
#include <optional>

namespace sentra
{

/**
 * \brief Baseline update strategy
 *
 * \ingroup sentra
 */
enum BaselineStrategy
{
    BS_MANUAL,
    BS_AUTO_ACCEPT_ON_BRANCH,
    BS_THRESHOLD,
    BS_AI_ASSISTED
};
I_DECLARE_ENUM(BaselineStrategy,
            BS_MANUAL,
            BS_AUTO_ACCEPT_ON_BRANCH,
            BS_THRESHOLD,
            BS_AI_ASSISTED);

/**
 * \brief Baseline decision for a specific diff
 *
 * \ingroup sentra
 */
enum BaselineDecision
{
    BD_ACCEPT,
    BD_REJECT,
    BD_IGNORE_REGION
};
I_DECLARE_ENUM(BaselineDecision,
            BD_ACCEPT,
            BD_REJECT,
            BD_IGNORE_REGION);

/**
 * \brief Baseline entry metadata
 */
struct BaselineEntry {
    QString id;
    QString scenarioId;
    QString nodeId;
    QString screenshotPath;
    std::optional<QString> semanticSnapshotPath;
    std::optional<QString> layoutMetadataPath;
    QString createdAt;
    QString updatedAt;
    int version = 1;
};

/**
 * \brief Interface for baseline management
 *
 * Manages baseline images and their lifecycle including versioning,
 * approval workflows, and update strategies.
 *
 * \ingroup sentra
 */
class IBaselineManager: virtual public istd::IPolymorphic
{
public:
    /**
     * \brief Get baseline for a specific scenario node
     */
    virtual std::optional<BaselineEntry> GetBaseline(
        const QString& scenarioId,
        const QString& nodeId
    ) const = 0;

    /**
     * \brief Create or update baseline from execution result
     * \param scenarioId Scenario identifier
     * \param nodeId Node identifier
     * \param screenshotPath Path to the new baseline screenshot
     * \return Created/updated baseline entry
     */
    virtual BaselineEntry SetBaseline(
        const QString& scenarioId,
        const QString& nodeId,
        const QString& screenshotPath
    ) = 0;

    /**
     * \brief Apply decision to a baseline diff
     */
    virtual void ApplyDecision(
        const QString& baselineId,
        BaselineDecision decision,
        const QString& decidedBy
    ) = 0;

    /**
     * \brief Get all baselines for a scenario
     */
    virtual QList<BaselineEntry> GetBaselines(const QString& scenarioId) const = 0;

    /**
     * \brief Get baseline history (all versions)
     */
    virtual QList<BaselineEntry> GetHistory(
        const QString& scenarioId,
        const QString& nodeId
    ) const = 0;

    /**
     * \brief Get current update strategy
     */
    virtual BaselineStrategy GetStrategy() const = 0;

    /**
     * \brief Set update strategy
     */
    virtual void SetStrategy(BaselineStrategy strategy) = 0;
};

typedef istd::TUniqueInterfacePtr<IBaselineManager> IBaselineManagerUniquePtr;

} // namespace sentra
