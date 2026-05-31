#pragma once

#include <string>
#include <vector>
#include <optional>

namespace imtsentra {

/**
 * @brief Baseline update strategy
 */
enum class BaselineStrategy {
    Manual,
    AutoAcceptOnBranch,
    Threshold,
    AiAssisted
};

/**
 * @brief Baseline decision for a specific diff
 */
enum class BaselineDecision {
    Accept,
    Reject,
    IgnoreRegion
};

/**
 * @brief Baseline entry metadata
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
 * @brief Interface for baseline management
 *
 * Manages baseline images and their lifecycle including versioning,
 * approval workflows, and update strategies.
 */
class IBaselineManager {
public:
    virtual ~IBaselineManager() = default;

    /**
     * @brief Get baseline for a specific scenario node
     */
    virtual std::optional<BaselineEntry> getBaseline(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const = 0;

    /**
     * @brief Create or update baseline from execution result
     * @param scenarioId Scenario identifier
     * @param nodeId Node identifier
     * @param screenshotPath Path to the new baseline screenshot
     * @return Created/updated baseline entry
     */
    virtual BaselineEntry setBaseline(
        const std::string& scenarioId,
        const std::string& nodeId,
        const std::string& screenshotPath
    ) = 0;

    /**
     * @brief Apply decision to a baseline diff
     */
    virtual void applyDecision(
        const std::string& baselineId,
        BaselineDecision decision,
        const std::string& decidedBy
    ) = 0;

    /**
     * @brief Get all baselines for a scenario
     */
    virtual std::vector<BaselineEntry> getBaselines(const std::string& scenarioId) const = 0;

    /**
     * @brief Get baseline history (all versions)
     */
    virtual std::vector<BaselineEntry> getHistory(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const = 0;

    /**
     * @brief Get current update strategy
     */
    virtual BaselineStrategy getStrategy() const = 0;

    /**
     * @brief Set update strategy
     */
    virtual void setStrategy(BaselineStrategy strategy) = 0;
};

} // namespace imtsentra
