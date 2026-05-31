#pragma once

#include "IScenarioExecutor.h"
#include <mutex>
#include <unordered_map>

namespace imtsentra {

/**
 * @brief ACF Component implementing IScenarioExecutor
 *
 * Executes scenario graphs by traversing nodes in topological order,
 * delegating each node to the appropriate INodeAction implementation.
 */
class CScenarioExecutorComp : public IScenarioExecutor {
public:
    CScenarioExecutorComp();
    ~CScenarioExecutorComp() override;

    std::string execute(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) override;

    void stop(const std::string& executionId) override;

    std::string retry(
        const std::string& executionId,
        const std::string& fromNodeId
    ) override;

    ExecutionStatus getStatus(const std::string& executionId) const override;
    std::vector<NodeExecutionResult> getResults(const std::string& executionId) const override;
    void onProgress(ExecutionProgressCallback callback) override;

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

    std::string generateId() const;
    void notifyProgress(const std::string& executionId, const std::string& nodeId,
                       ExecutionStatus status, float progress);
};

} // namespace imtsentra
