// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <imtsentra/IBaselineManager.h>
#include <unordered_map>
#include <mutex>

namespace imtsentra
{

/**
 * \brief ACF Component implementing IBaselineManager
 *
 * Manages baseline images with versioning, approval workflows,
 * and configurable update strategies.
 */
class CBaselineManagerComp : public IBaselineManager {
public:
    CBaselineManagerComp();
    ~CBaselineManagerComp() override;

    std::optional<BaselineEntry> GetBaseline(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const override;

    BaselineEntry SetBaseline(
        const std::string& scenarioId,
        const std::string& nodeId,
        const std::string& screenshotPath
    ) override;

    void ApplyDecision(
        const std::string& baselineId,
        BaselineDecision decision,
        const std::string& decidedBy
    ) override;

    std::vector<BaselineEntry> GetBaselines(const std::string& scenarioId) const override;

    std::vector<BaselineEntry> GetHistory(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const override;

    BaselineStrategy GetStrategy() const override;
    void SetStrategy(BaselineStrategy strategy) override;

private:
    mutable std::mutex m_mutex;
    BaselineStrategy m_strategy = BS_MANUAL;
    // Key: "scenarioId/nodeId"
    std::unordered_map<std::string, std::vector<BaselineEntry>> m_baselines;

    std::string MakeKey(const std::string& scenarioId, const std::string& nodeId) const;
};

} // namespace imtsentra
