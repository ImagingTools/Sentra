#pragma once

#include "IExecutionEngine.h"
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace imtsentra {

/**
 * @brief ACF Component implementing IExecutionEngine
 *
 * Orchestrates scenario execution with support for parallel runs,
 * retry logic, and timeout management.
 */
class CExecutionEngineComp : public IExecutionEngine {
public:
    CExecutionEngineComp();
    ~CExecutionEngineComp() override;

    std::string queueExecution(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) override;

    std::vector<std::string> queueParallelExecution(
        const std::vector<std::shared_ptr<IScenarioGraph>>& graphs,
        const ExecutionConfig& config
    ) override;

    void cancelExecution(const std::string& executionId) override;
    int getActiveExecutionCount() const override;
    EngineConfig getConfig() const override;
    void setConfig(const EngineConfig& config) override;

    /**
     * @brief Start the execution engine worker threads
     */
    void start();

    /**
     * @brief Stop the engine and wait for running executions to complete
     */
    void stop();

private:
    struct QueuedExecution {
        std::string id;
        std::shared_ptr<IScenarioGraph> graph;
        ExecutionConfig config;
    };

    EngineConfig m_config;
    mutable std::mutex m_mutex;
    std::queue<QueuedExecution> m_queue;
    std::vector<std::thread> m_workers;
    std::condition_variable m_cv;
    std::atomic<bool> m_running{false};
    std::atomic<int> m_activeCount{0};

    void workerLoop();
    std::string generateId() const;
};

} // namespace imtsentra
