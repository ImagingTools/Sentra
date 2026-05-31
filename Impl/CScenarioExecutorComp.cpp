// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CScenarioExecutorComp.h>
#include <imtsentra/IScenarioGraph.h>
#include <chrono>
#include <random>

namespace imtsentra
{

CScenarioExecutorComp::CScenarioExecutorComp() = default;
CScenarioExecutorComp::~CScenarioExecutorComp() = default;

std::string CScenarioExecutorComp::Execute(
    std::shared_ptr<IScenarioGraph> graph,
    const ExecutionConfig& config
) {
    std::lock_guard<std::mutex> lock(m_mutex);

    ExecutionState state;
    state.id = GenerateId();
    state.status = ES_RUNNING;
    state.graph = graph;
    state.config = config;

    std::string executionId = state.id;
    m_executions[executionId] = std::move(state);
    NotifyProgress(executionId, "", ES_RUNNING, 0.0f);

    // TODO: Actual execution logic
    // 1. Get topological order of nodes
    // 2. For each node:
    //    a. Resolve intent via AI
    //    b. Execute action via Browser Controller
    //    c. Capture screenshot + DOM
    //    d. Validate assertions
    //    e. Record NodeExecutionResult

    return executionId;
}

void CScenarioExecutorComp::Stop(const std::string& executionId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_executions.find(executionId);
    if (it != m_executions.end()) {
        it->second.stopRequested = true;
        it->second.status = ES_FAILED;
    }
}

std::string CScenarioExecutorComp::Retry(
    const std::string& executionId,
    const std::string& fromNodeId
) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_executions.find(executionId);
    if (it == m_executions.end()) {
        throw std::runtime_error("Execution not found: " + executionId);
    }

    // Create new execution starting from the specified node
    return Execute(it->second.graph, it->second.config);
}

ExecutionStatus CScenarioExecutorComp::GetStatus(const std::string& executionId) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_executions.find(executionId);
    if (it != m_executions.end()) {
        return it->second.status;
    }
    return ES_FAILED;
}

std::vector<NodeExecutionResult> CScenarioExecutorComp::GetResults(const std::string& executionId) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_executions.find(executionId);
    if (it != m_executions.end()) {
        return it->second.results;
    }
    return {};
}

void CScenarioExecutorComp::OnProgress(ExecutionProgressCallback callback) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_callbacks.push_back(std::move(callback));
}

std::string CScenarioExecutorComp::GenerateId() const {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return "exec-" + std::to_string(ms);
}

void CScenarioExecutorComp::NotifyProgress(
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
