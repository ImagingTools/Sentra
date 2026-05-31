// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <imtsentra/IScenarioExecutor.h>
#include <mutex>
#include <unordered_map>

namespace imtsentra
{

/**
 * \brief ACF Component implementing IScenarioExecutor
 *
 * Executes scenario graphs by traversing nodes in topological order,
 * delegating each node to the appropriate INodeAction implementation.
 */
class CScenarioExecutorComp : public IScenarioExecutor {
public:
    CScenarioExecutorComp();
    ~CScenarioExecutorComp() override;

    std::string Execute(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) override;

    void Stop(const std::string& executionId) override;

    std::string Retry(
        const std::string& executionId,
        const std::string& fromNodeId
    ) override;

    ExecutionStatus GetStatus(const std::string& executionId) const override;
    std::vector<NodeExecutionResult> GetResults(const std::string& executionId) const override;
    void OnProgress(ExecutionProgressCallback callback) override;

private:
    struct ExecutionState {
        std::string id;
        ExecutionStatus status;
        std::shared_ptr<IScenarioGraph> graph;
        ExecutionConfig config;
        std::vector<NodeExecutionResult> results;
        bool stopRequested = false;
    };

    mutable std::mutex m_mutex;
    std::unordered_map<std::string, ExecutionState> m_executions;
    std::vector<ExecutionProgressCallback> m_callbacks;

    std::string GenerateId() const;
    void NotifyProgress(const std::string& executionId, const std::string& nodeId,
                       ExecutionStatus status, float progress);
};

} // namespace imtsentra
