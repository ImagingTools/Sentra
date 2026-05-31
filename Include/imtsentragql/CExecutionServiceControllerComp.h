// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <string>
#include <vector>
#include <functional>

namespace imtsentra
{
namespace gql
{

/**
 * \brief GraphQL service controller for Execution operations
 *
 * Handles:
 * - ExecuteScenario(scenarioId, config) → ExecutionId
 * - GetExecution(executionId) → Execution
 * - GetNodeResult(executionId, nodeId) → NodeExecutionResult
 * - StopExecution(executionId)
 * - RetryExecution(executionId, fromNodeId)
 * - Subscriptions: onExecutionProgress, onNodeCompleted
 */
class CExecutionServiceControllerComp {
public:
    CExecutionServiceControllerComp();
    ~CExecutionServiceControllerComp();

    // Query resolvers
    std::string GetExecution(const std::string& executionId) const;
    std::string GetNodeResult(const std::string& executionId, const std::string& nodeId) const;
    std::string ListExecutions(const std::string& scenarioId) const;

    // Mutation resolvers
    std::string ExecuteScenario(const std::string& scenarioId, const std::string& configJson);
    bool StopExecution(const std::string& executionId);
    std::string RetryExecution(const std::string& executionId, const std::string& fromNodeId);

    // Subscription handlers
    using ProgressHandler = std::function<void(const std::string& progressJson)>;
    void SubscribeExecutionProgress(const std::string& executionId, ProgressHandler handler);
    void SubscribeNodeCompleted(const std::string& executionId, ProgressHandler handler);
};

} // namespace gql
} // namespace imtsentra
