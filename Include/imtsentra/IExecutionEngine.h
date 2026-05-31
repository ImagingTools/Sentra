#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace imtsentra {

class IScenarioGraph;
struct ExecutionConfig;
struct NodeExecutionResult;
enum class ExecutionStatus;

/**
 * @brief Execution engine configuration
 */
struct EngineConfig {
    int maxParallelExecutions = 4;
    int defaultTimeoutMs = 30000;
    int maxRetries = 2;
    bool captureScreenshotsOnFailure = true;
    bool captureDomSnapshots = true;
};

/**
 * @brief Interface for the execution engine
 *
 * Orchestrates scenario execution including parallel execution,
 * retry logic, timeout management, and conditional branching.
 */
class IExecutionEngine {
public:
    virtual ~IExecutionEngine() = default;

    /**
     * @brief Queue a scenario for execution
     * @param graph Scenario graph to execute
     * @param config Execution configuration
     * @return Execution ID
     */
    virtual std::string queueExecution(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) = 0;

    /**
     * @brief Execute multiple scenarios in parallel
     * @param graphs List of scenario graphs
     * @param config Shared execution configuration
     * @return List of execution IDs
     */
    virtual std::vector<std::string> queueParallelExecution(
        const std::vector<std::shared_ptr<IScenarioGraph>>& graphs,
        const ExecutionConfig& config
    ) = 0;

    /**
     * @brief Cancel a queued or running execution
     */
    virtual void cancelExecution(const std::string& executionId) = 0;

    /**
     * @brief Get the number of currently running executions
     */
    virtual int getActiveExecutionCount() const = 0;

    /**
     * @brief Get engine configuration
     */
    virtual EngineConfig getConfig() const = 0;

    /**
     * @brief Update engine configuration
     */
    virtual void setConfig(const EngineConfig& config) = 0;
};

} // namespace imtsentra
