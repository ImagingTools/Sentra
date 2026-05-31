// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <istd/istd.h>
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Standard includes
#include <string>
#include <vector>
#include <optional>

namespace imtsentra
{

/**
 * \brief Baseline update strategy
 *
 * \ingroup imtsentra
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
 * \ingroup imtsentra
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
    std::string id;
    std::string scenarioId;
    std::string nodeId;
    std::string screenshotPath;
    std::optional<std::string> semanticSnapshotPath;
    std::optional<std::string> layoutMetadataPath;
    std::string createdAt;
    std::string updatedAt;
    int version = 1;
};

/**
 * \brief Interface for baseline management
 *
 * Manages baseline images and their lifecycle including versioning,
 * approval workflows, and update strategies.
 *
 * \ingroup imtsentra
 */
class IBaselineManager: virtual public istd::IPolymorphic
{
public:
    /**
     * \brief Get baseline for a specific scenario node
     */
    virtual std::optional<BaselineEntry> GetBaseline(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const = 0;

    /**
     * \brief Create or update baseline from execution result
     * \param scenarioId Scenario identifier
     * \param nodeId Node identifier
     * \param screenshotPath Path to the new baseline screenshot
     * \return Created/updated baseline entry
     */
    virtual BaselineEntry SetBaseline(
        const std::string& scenarioId,
        const std::string& nodeId,
        const std::string& screenshotPath
    ) = 0;

    /**
     * \brief Apply decision to a baseline diff
     */
    virtual void ApplyDecision(
        const std::string& baselineId,
        BaselineDecision decision,
        const std::string& decidedBy
    ) = 0;

    /**
     * \brief Get all baselines for a scenario
     */
    virtual std::vector<BaselineEntry> GetBaselines(const std::string& scenarioId) const = 0;

    /**
     * \brief Get baseline history (all versions)
     */
    virtual std::vector<BaselineEntry> GetHistory(
        const std::string& scenarioId,
        const std::string& nodeId
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

} // namespace imtsentra
