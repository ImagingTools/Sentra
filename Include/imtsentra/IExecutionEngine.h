// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <imtsentra/IScenarioExecutor.h>
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Standard includes
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace imtsentra
{

class IScenarioGraph;

/**
 * \brief Execution engine configuration
 */
struct EngineConfig {
    int maxParallelExecutions = 4;
    int defaultTimeoutMs = 30000;
    int maxRetries = 2;
    bool captureScreenshotsOnFailure = true;
    bool captureDomSnapshots = true;
};

/**
 * \brief Interface for the execution engine
 *
 * Orchestrates scenario execution including parallel execution,
 * retry logic, timeout management, and conditional branching.
 *
 * \ingroup imtsentra
 */
class IExecutionEngine: virtual public istd::IPolymorphic
{
public:
    /**
     * \brief Queue a scenario for execution
     * \param graph Scenario graph to execute
     * \param config Execution configuration
     * \return Execution ID
     */
    virtual std::string QueueExecution(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) = 0;

    /**
     * \brief Execute multiple scenarios in parallel
     * \param graphs List of scenario graphs
     * \param config Shared execution configuration
     * \return List of execution IDs
     */
    virtual std::vector<std::string> QueueParallelExecution(
        const std::vector<std::shared_ptr<IScenarioGraph>>& graphs,
        const ExecutionConfig& config
    ) = 0;

    /**
     * \brief Cancel a queued or running execution
     */
    virtual void CancelExecution(const std::string& executionId) = 0;

    /**
     * \brief Get the number of currently running executions
     */
    virtual int GetActiveExecutionCount() const = 0;

    /**
     * \brief Get engine configuration
     */
    virtual EngineConfig GetConfig() const = 0;

    /**
     * \brief Update engine configuration
     */
    virtual void SetConfig(const EngineConfig& config) = 0;
};

typedef istd::TUniqueInterfacePtr<IExecutionEngine> IExecutionEngineUniquePtr;

} // namespace imtsentra
