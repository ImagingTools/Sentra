// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// ImtSentra includes
#include <imtsentra/IExecutionEngine.h>

// Standard includes
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace imtsentra
{

/**
 * \brief ACF component implementing IExecutionEngine
 *
 * Orchestrates scenario execution with support for parallel runs,
 * retry logic, and timeout management.
 *
 * \ingroup imtsentra
 */
class CExecutionEngineComp:
        public icomp::CComponentBase,
        virtual public IExecutionEngine
{
public:
    typedef icomp::CComponentBase BaseClass;

    I_BEGIN_COMPONENT(CExecutionEngineComp)
        I_REGISTER_INTERFACE(IExecutionEngine);
    I_END_COMPONENT

    // reimplemented (imtsentra::IExecutionEngine)
    std::string QueueExecution(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) override;

    std::vector<std::string> QueueParallelExecution(
        const std::vector<std::shared_ptr<IScenarioGraph>>& graphs,
        const ExecutionConfig& config
    ) override;

    void CancelExecution(const std::string& executionId) override;
    int GetActiveExecutionCount() const override;
    EngineConfig GetConfig() const override;
    void SetConfig(const EngineConfig& config) override;

    /**
     * \brief Start the execution engine worker threads
     */
    void Start();

    /**
     * \brief Stop the engine and wait for running executions to complete
     */
    void Stop();

protected:
    // reimplemented (icomp::CComponentBase)
    void OnComponentDestroyed() override;

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

    void WorkerLoop();
    std::string GenerateId() const;
};

} // namespace imtsentra
