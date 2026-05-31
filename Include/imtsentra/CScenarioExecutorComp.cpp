// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CScenarioExecutorComp.h>
#include <imtsentra/IScenarioGraph.h>

// Qt includes
#include <QtCore/QDateTime>
#include <QtCore/QMutexLocker>

// Standard includes
#include <stdexcept>

namespace imtsentra
{

QString CScenarioExecutorComp::Execute(
    std::shared_ptr<IScenarioGraph> graph,
    const ExecutionConfig& config
) {
    QMutexLocker lock(&m_mutex);

    ExecutionState state;
    state.id = GenerateId();
    state.status = ES_RUNNING;
    state.graph = graph;
    state.config = config;

    QString executionId = state.id;
    m_executions[executionId] = std::move(state);
    NotifyProgress(executionId, QString(), ES_RUNNING, 0.0f);

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

void CScenarioExecutorComp::Stop(const QString& executionId) {
    QMutexLocker lock(&m_mutex);
    auto it = m_executions.find(executionId);
    if (it != m_executions.end()) {
        it.value().stopRequested = true;
        it.value().status = ES_FAILED;
    }
}

QString CScenarioExecutorComp::Retry(
    const QString& executionId,
    const QString& fromNodeId
) {
    QMutexLocker lock(&m_mutex);
    auto it = m_executions.find(executionId);
    if (it == m_executions.end()) {
        throw std::runtime_error(
            QStringLiteral("Execution not found: %1").arg(executionId).toStdString());
    }

    // Create new execution starting from the specified node
    return Execute(it.value().graph, it.value().config);
}

ExecutionStatus CScenarioExecutorComp::GetStatus(const QString& executionId) const {
    QMutexLocker lock(&m_mutex);
    auto it = m_executions.find(executionId);
    if (it != m_executions.end()) {
        return it.value().status;
    }
    return ES_FAILED;
}

QList<NodeExecutionResult> CScenarioExecutorComp::GetResults(const QString& executionId) const {
    QMutexLocker lock(&m_mutex);
    auto it = m_executions.find(executionId);
    if (it != m_executions.end()) {
        return it.value().results;
    }
    return {};
}

void CScenarioExecutorComp::OnProgress(ExecutionProgressCallback callback) {
    QMutexLocker lock(&m_mutex);
    m_callbacks.push_back(std::move(callback));
}

QString CScenarioExecutorComp::GenerateId() const {
    return QStringLiteral("exec-") + QString::number(QDateTime::currentMSecsSinceEpoch());
}

void CScenarioExecutorComp::NotifyProgress(
    const QString& executionId,
    const QString& nodeId,
    ExecutionStatus status,
    float progress
) {
    for (const auto& callback : m_callbacks) {
        callback(executionId, nodeId, status, progress);
    }
}

} // namespace imtsentra
