#pragma once

#include "IBaselineManager.h"
#include <unordered_map>
#include <mutex>

namespace imtsentra {

/**
 * @brief ACF Component implementing IBaselineManager
 *
 * Manages baseline images with versioning, approval workflows,
 * and configurable update strategies.
 */
class CBaselineManagerComp : public IBaselineManager {
public:
    CBaselineManagerComp();
    ~CBaselineManagerComp() override;

    std::optional<BaselineEntry> getBaseline(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const override;

    BaselineEntry setBaseline(
        const std::string& scenarioId,
        const std::string& nodeId,
        const std::string& screenshotPath
    ) override;

    void applyDecision(
        const std::string& baselineId,
        BaselineDecision decision,
        const std::string& decidedBy
    ) override;

    std::vector<BaselineEntry> getBaselines(const std::string& scenarioId) const override;

    std::vector<BaselineEntry> getHistory(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const override;

    BaselineStrategy getStrategy() const override;
    void setStrategy(BaselineStrategy strategy) override;

private:
    mutable std::mutex m_mutex;
    BaselineStrategy m_strategy = BaselineStrategy::Manual;
    // Key: "scenarioId/nodeId"
    std::unordered_map<std::string, std::vector<BaselineEntry>> m_baselines;

    std::string makeKey(const std::string& scenarioId, const std::string& nodeId) const;
};

} // namespace imtsentra
