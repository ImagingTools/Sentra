#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <optional>

namespace imtsentra {

/**
 * @brief Execution status for individual nodes and overall execution
 */
enum class ExecutionStatus {
    Pending,
    Running,
    Passed,
    Failed,
    Skipped,
    TimedOut
};

/**
 * @brief Result of executing a single scenario node
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
 * @brief Execution configuration
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
 * @brief Progress callback for execution monitoring
 */
using ExecutionProgressCallback = std::function<void(
    const std::string& executionId,
    const std::string& nodeId,
    ExecutionStatus status,
    float overallProgress
)>;

class IScenarioGraph;

/**
 * @brief Interface for scenario execution
 *
 * Executes a scenario graph by traversing nodes in topological order,
 * resolving intents via AI, performing browser actions, and collecting results.
 */
class IScenarioExecutor {
public:
    virtual ~IScenarioExecutor() = default;

    /**
     * @brief Start executing a scenario
     * @param graph The scenario graph to execute
     * @param config Execution configuration
     * @return Execution ID for tracking
     */
    virtual std::string execute(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) = 0;

    /**
     * @brief Stop a running execution
     * @param executionId ID of the execution to stop
     */
    virtual void stop(const std::string& executionId) = 0;

    /**
     * @brief Retry execution from a specific node
     * @param executionId Original execution ID
     * @param fromNodeId Node to restart from
     * @return New execution ID
     */
    virtual std::string retry(
        const std::string& executionId,
        const std::string& fromNodeId
    ) = 0;

    /**
     * @brief Get current execution status
     */
    virtual ExecutionStatus getStatus(const std::string& executionId) const = 0;

    /**
     * @brief Get results for all completed nodes
     */
    virtual std::vector<NodeExecutionResult> getResults(const std::string& executionId) const = 0;

    /**
     * @brief Register progress callback
     */
    virtual void onProgress(ExecutionProgressCallback callback) = 0;
};

} // namespace imtsentra
