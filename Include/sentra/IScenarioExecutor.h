// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <istd/istd.h>
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Qt includes
#include <QtCore/QString>
#include <QtCore/QList>

// Standard includes
#include <functional>
#include <memory>
#include <optional>

namespace sentra
{

/**
 * \brief Execution status for individual nodes and overall execution
 *
 * \ingroup sentra
 */
enum ExecutionStatus
{
    ES_PENDING,
    ES_RUNNING,
    ES_PASSED,
    ES_FAILED,
    ES_SKIPPED,
    ES_TIMED_OUT
};
I_DECLARE_ENUM(ExecutionStatus,
            ES_PENDING,
            ES_RUNNING,
            ES_PASSED,
            ES_FAILED,
            ES_SKIPPED,
            ES_TIMED_OUT);

/**
 * \brief Result of executing a single scenario node
 */
struct NodeExecutionResult {
    QString nodeId;
    ExecutionStatus status;
    QString startedAt;
    QString completedAt;
    std::optional<QString> screenshotPath;
    std::optional<QString> domSnapshotPath;
    std::optional<QString> errorMessage;
    int durationMs = 0;
};

/**
 * \brief Execution configuration
 */
struct ExecutionConfig {
    QString environment;
    QString baseUrl;
    bool headless = true;
    int timeoutMs = 30000;
    int viewportWidth = 1920;
    int viewportHeight = 1080;
    std::optional<QString> credentials;
};

/**
 * \brief Progress callback for execution monitoring
 */
using ExecutionProgressCallback = std::function<void(
    const QString& executionId,
    const QString& nodeId,
    ExecutionStatus status,
    float overallProgress
)>;

class IScenarioGraph;

/**
 * \brief Interface for scenario execution
 *
 * Executes a scenario graph by traversing nodes in topological order,
 * resolving intents via AI, performing browser actions, and collecting results.
 *
 * \ingroup sentra
 */
class IScenarioExecutor: virtual public istd::IPolymorphic
{
public:
    /**
     * \brief Start executing a scenario
     * \param graph The scenario graph to execute
     * \param config Execution configuration
     * \return Execution ID for tracking
     */
    virtual QString Execute(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) = 0;

    /**
     * \brief Stop a running execution
     * \param executionId ID of the execution to stop
     */
    virtual void Stop(const QString& executionId) = 0;

    /**
     * \brief Retry execution from a specific node
     * \param executionId Original execution ID
     * \param fromNodeId Node to restart from
     * \return New execution ID
     */
    virtual QString Retry(
        const QString& executionId,
        const QString& fromNodeId
    ) = 0;

    /**
     * \brief Get current execution status
     */
    virtual ExecutionStatus GetStatus(const QString& executionId) const = 0;

    /**
     * \brief Get results for all completed nodes
     */
    virtual QList<NodeExecutionResult> GetResults(const QString& executionId) const = 0;

    /**
     * \brief Register progress callback
     */
    virtual void OnProgress(ExecutionProgressCallback callback) = 0;
};

typedef istd::TUniqueInterfacePtr<IScenarioExecutor> IScenarioExecutorUniquePtr;

} // namespace sentra
