#pragma once

#include <string>
#include <vector>
#include <functional>

namespace imtsentra {
namespace gql {

/**
 * @brief GraphQL service controller for Execution operations
 *
 * Handles:
 * - executeScenario(scenarioId, config) → ExecutionId
 * - getExecution(executionId) → Execution
 * - getNodeResult(executionId, nodeId) → NodeExecutionResult
 * - stopExecution(executionId)
 * - retryExecution(executionId, fromNodeId)
 * - Subscriptions: onExecutionProgress, onNodeCompleted
 */
class CExecutionServiceControllerComp {
public:
    CExecutionServiceControllerComp();
    ~CExecutionServiceControllerComp();

    // Query resolvers
    std::string getExecution(const std::string& executionId) const;
    std::string getNodeResult(const std::string& executionId, const std::string& nodeId) const;
    std::string listExecutions(const std::string& scenarioId) const;

    // Mutation resolvers
    std::string executeScenario(const std::string& scenarioId, const std::string& configJson);
    bool stopExecution(const std::string& executionId);
    std::string retryExecution(const std::string& executionId, const std::string& fromNodeId);

    // Subscription handlers
    using ProgressHandler = std::function<void(const std::string& progressJson)>;
    void subscribeExecutionProgress(const std::string& executionId, ProgressHandler handler);
    void subscribeNodeCompleted(const std::string& executionId, ProgressHandler handler);
};

} // namespace gql
} // namespace imtsentra
