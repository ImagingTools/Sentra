#include "imtsentra/CExecutionEngineComp.h"
#include "imtsentra/IScenarioExecutor.h"
#include <chrono>

namespace imtsentra {

CExecutionEngineComp::CExecutionEngineComp() = default;

CExecutionEngineComp::~CExecutionEngineComp() {
    stop();
}

std::string CExecutionEngineComp::queueExecution(
    std::shared_ptr<IScenarioGraph> graph,
    const ExecutionConfig& config
) {
    std::lock_guard<std::mutex> lock(m_mutex);
    QueuedExecution qe;
    qe.id = generateId();
    qe.graph = graph;
    qe.config = config;
    m_queue.push(std::move(qe));
    m_cv.notify_one();
    return qe.id;
}

std::vector<std::string> CExecutionEngineComp::queueParallelExecution(
    const std::vector<std::shared_ptr<IScenarioGraph>>& graphs,
    const ExecutionConfig& config
) {
    std::vector<std::string> ids;
    ids.reserve(graphs.size());
    for (const auto& graph : graphs) {
        ids.push_back(queueExecution(graph, config));
    }
    return ids;
}

void CExecutionEngineComp::cancelExecution(const std::string& executionId) {
    // TODO: Signal cancellation to running execution
}

int CExecutionEngineComp::getActiveExecutionCount() const {
    return m_activeCount.load();
}

EngineConfig CExecutionEngineComp::getConfig() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_config;
}

void CExecutionEngineComp::setConfig(const EngineConfig& config) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_config = config;
}

void CExecutionEngineComp::start() {
    m_running = true;
    for (int i = 0; i < m_config.maxParallelExecutions; ++i) {
        m_workers.emplace_back(&CExecutionEngineComp::workerLoop, this);
    }
}

void CExecutionEngineComp::stop() {
    m_running = false;
    m_cv.notify_all();
    for (auto& worker : m_workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    m_workers.clear();
}

void CExecutionEngineComp::workerLoop() {
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

std::string CExecutionEngineComp::generateId() const {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return "exec-" + std::to_string(ms);
}

} // namespace imtsentra
