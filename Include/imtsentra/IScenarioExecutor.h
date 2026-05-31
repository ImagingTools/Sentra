// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <optional>

namespace imtsentra
{

/**
 * \brief Execution status for individual nodes and overall execution
 */
enum ExecutionStatus
{
    ES_PENDING,
    ES_RUNNING,
    ES_PASSED,
    ES_FAILED,
    ES_SKIPPED,
    ES_TIMED_OUT
};

/**
 * \brief Result of executing a single scenario node
 */
struct NodeExecutionResult {
    std::string nodeId;
    ExecutionStatus status;
    std::string startedAt;
    std::string completedAt;
    std::optional<std::string> screenshotPath;
    std::optional<std::string> domSnapshotPath;
    std::optional<std::string> errorMessage;
    int durationMs = 0;
};

/**
 * \brief Execution configuration
 */
struct ExecutionConfig {
    std::string environment;
    std::string baseUrl;
    bool headless = true;
    int timeoutMs = 30000;
    int viewportWidth = 1920;
    int viewportHeight = 1080;
    std::optional<std::string> credentials;
};

/**
 * \brief Progress callback for execution monitoring
 */
using ExecutionProgressCallback = std::function<void(
    const std::string& executionId,
    const std::string& nodeId,
    ExecutionStatus status,
    float overallProgress
)>;

class IScenarioGraph;

/**
 * \brief Interface for scenario execution
 *
 * Executes a scenario graph by traversing nodes in topological order,
 * resolving intents via AI, performing browser actions, and collecting results.
 */
class IScenarioExecutor {
public:
    virtual ~IScenarioExecutor() = default;

    /**
     * \brief Start executing a scenario
     * \param graph The scenario graph to execute
     * \param config Execution configuration
     * \return Execution ID for tracking
     */
    virtual std::string Execute(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) = 0;

    /**
     * \brief Stop a running execution
     * \param executionId ID of the execution to stop
     */
    virtual void Stop(const std::string& executionId) = 0;

    /**
     * \brief Retry execution from a specific node
     * \param executionId Original execution ID
     * \param fromNodeId Node to restart from
     * \return New execution ID
     */
    virtual std::string Retry(
        const std::string& executionId,
        const std::string& fromNodeId
    ) = 0;

    /**
     * \brief Get current execution status
     */
    virtual ExecutionStatus GetStatus(const std::string& executionId) const = 0;

    /**
     * \brief Get results for all completed nodes
     */
    virtual std::vector<NodeExecutionResult> GetResults(const std::string& executionId) const = 0;

    /**
     * \brief Register progress callback
     */
    virtual void OnProgress(ExecutionProgressCallback callback) = 0;
};

} // namespace imtsentra
