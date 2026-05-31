// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CExecutionEngineComp.h>
#include <imtsentra/IScenarioExecutor.h>
#include <chrono>

namespace imtsentra
{

CExecutionEngineComp::CExecutionEngineComp() = default;

CExecutionEngineComp::~CExecutionEngineComp() {
    Stop();
}

std::string CExecutionEngineComp::QueueExecution(
    std::shared_ptr<IScenarioGraph> graph,
    const ExecutionConfig& config
) {
    std::lock_guard<std::mutex> lock(m_mutex);
    QueuedExecution qe;
    qe.id = GenerateId();
    qe.graph = graph;
    qe.config = config;
    std::string id = qe.id;
    m_queue.push(std::move(qe));
    m_cv.notify_one();
    return id;
}

std::vector<std::string> CExecutionEngineComp::QueueParallelExecution(
    const std::vector<std::shared_ptr<IScenarioGraph>>& graphs,
    const ExecutionConfig& config
) {
    std::vector<std::string> ids;
    ids.reserve(graphs.size());
    for (const auto& graph : graphs) {
        ids.push_back(QueueExecution(graph, config));
    }
    return ids;
}

void CExecutionEngineComp::CancelExecution(const std::string& executionId) {
    // TODO: Signal cancellation to running execution
}

int CExecutionEngineComp::GetActiveExecutionCount() const {
    return m_activeCount.load();
}

EngineConfig CExecutionEngineComp::GetConfig() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_config;
}

void CExecutionEngineComp::SetConfig(const EngineConfig& config) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_config = config;
}

void CExecutionEngineComp::Start() {
    m_running = true;
    for (int i = 0; i < m_config.maxParallelExecutions; ++i) {
        m_workers.emplace_back(&CExecutionEngineComp::WorkerLoop, this);
    }
}

void CExecutionEngineComp::Stop() {
    m_running = false;
    m_cv.notify_all();
    for (auto& worker : m_workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    m_workers.clear();
}

void CExecutionEngineComp::WorkerLoop() {
    while (m_running) {
        QueuedExecution qe;
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this] {
                return !m_queue.empty() || !m_running;
            });
            if (!m_running) return;
            if (m_queue.empty()) continue;
            qe = std::move(m_queue.front());
            m_queue.pop();
        }

        m_activeCount++;
        // TODO: Execute the scenario using CScenarioExecutorComp
        m_activeCount--;
    }
}

std::string CExecutionEngineComp::GenerateId() const {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return "exec-" + std::to_string(ms);
}

} // namespace imtsentra
