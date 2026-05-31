#include "imtsentra/CScenarioExecutorComp.h"
#include "imtsentra/IScenarioGraph.h"
#include <chrono>
#include <random>

namespace imtsentra {

CScenarioExecutorComp::CScenarioExecutorComp() = default;
CScenarioExecutorComp::~CScenarioExecutorComp() = default;

std::string CScenarioExecutorComp::execute(
    std::shared_ptr<IScenarioGraph> graph,
    const ExecutionConfig& config
) {
    std::lock_guard<std::mutex> lock(m_mutex);

    ExecutionState state;
    state.id = generateId();
    state.status = ExecutionStatus::Running;
    state.graph = graph;
    state.config = config;

    m_executions[state.id] = std::move(state);
    notifyProgress(state.id, "", ExecutionStatus::Running, 0.0f);

    // TODO: Actual execution logic
    // 1. Get topological order of nodes
    // 2. For each node:
    //    a. Resolve intent via AI
    //    b. Execute action via Browser Controller
    //    c. Capture screenshot + DOM
    //    d. Validate assertions
    //    e. Record NodeExecutionResult

    return m_executions.rbegin()->second.id;
}

void CScenarioExecutorComp::stop(const std::string& executionId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_executions.find(executionId);
    if (it != m_executions.end()) {
        it->second.stopRequested = true;
        it->second.status = ExecutionStatus::Failed;
    }
}

std::string CScenarioExecutorComp::retry(
    const std::string& executionId,
    const std::string& fromNodeId
) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_executions.find(executionId);
    if (it == m_executions.end()) {
        throw std::runtime_error("Execution not found: " + executionId);
    }

    // Create new execution starting from the specified node
    return execute(it->second.graph, it->second.config);
}

ExecutionStatus CScenarioExecutorComp::getStatus(const std::string& executionId) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_executions.find(executionId);
    if (it != m_executions.end()) {
        return it->second.status;
    }
    return ExecutionStatus::Failed;
}

std::vector<NodeExecutionResult> CScenarioExecutorComp::getResults(const std::string& executionId) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_executions.find(executionId);
    if (it != m_executions.end()) {
        return it->second.results;
    }
    return {};
}

void CScenarioExecutorComp::onProgress(ExecutionProgressCallback callback) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_callbacks.push_back(std::move(callback));
}

std::string CScenarioExecutorComp::generateId() const {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return "exec-" + std::to_string(ms);
}

void CScenarioExecutorComp::notifyProgress(
    const std::string& executionId,
    const std::string& nodeId,
    ExecutionStatus status,
    float progress
) {
    for (const auto& callback : m_callbacks) {
        callback(executionId, nodeId, status, progress);
    }
}

} // namespace imtsentra
