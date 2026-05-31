// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CExecutionEngineComp.h>
#include <imtsentra/IScenarioExecutor.h>

// Qt includes
#include <QtCore/QDateTime>
#include <QtCore/QMutexLocker>
#include <QtCore/QThread>

namespace imtsentra
{

void CExecutionEngineComp::OnComponentDestroyed() {
    Stop();

    BaseClass::OnComponentDestroyed();
}

QString CExecutionEngineComp::QueueExecution(
    std::shared_ptr<IScenarioGraph> graph,
    const ExecutionConfig& config
) {
    QMutexLocker lock(&m_mutex);
    QueuedExecution qe;
    qe.id = GenerateId();
    qe.graph = graph;
    qe.config = config;
    QString id = qe.id;
    m_queue.enqueue(std::move(qe));
    m_cv.wakeOne();
    return id;
}

QList<QString> CExecutionEngineComp::QueueParallelExecution(
    const QList<std::shared_ptr<IScenarioGraph>>& graphs,
    const ExecutionConfig& config
) {
    QList<QString> ids;
    ids.reserve(graphs.size());
    for (const auto& graph : graphs) {
        ids.push_back(QueueExecution(graph, config));
    }
    return ids;
}

void CExecutionEngineComp::CancelExecution(const QString& executionId) {
    // TODO: Signal cancellation to running execution
}

int CExecutionEngineComp::GetActiveExecutionCount() const {
    return m_activeCount.loadRelaxed();
}

EngineConfig CExecutionEngineComp::GetConfig() const {
    QMutexLocker lock(&m_mutex);
    return m_config;
}

void CExecutionEngineComp::SetConfig(const EngineConfig& config) {
    QMutexLocker lock(&m_mutex);
    m_config = config;
}

void CExecutionEngineComp::Start() {
    m_running.storeRelaxed(1);
    for (int i = 0; i < m_config.maxParallelExecutions; ++i) {
        QThread* worker = QThread::create([this] { WorkerLoop(); });
        m_workers.push_back(worker);
        worker->start();
    }
}

void CExecutionEngineComp::Stop() {
    m_running.storeRelaxed(0);
    m_cv.wakeAll();
    for (QThread* worker : m_workers) {
        if (worker != nullptr) {
            worker->wait();
            delete worker;
        }
    }
    m_workers.clear();
}

void CExecutionEngineComp::WorkerLoop() {
    while (m_running.loadRelaxed() != 0) {
        QueuedExecution qe;
        {
            QMutexLocker lock(&m_mutex);
            while (m_queue.isEmpty() && m_running.loadRelaxed() != 0) {
                m_cv.wait(&m_mutex);
            }
            if (m_running.loadRelaxed() == 0) return;
            if (m_queue.isEmpty()) continue;
            qe = m_queue.dequeue();
        }

        m_activeCount.ref();
        // TODO: Execute the scenario using CScenarioExecutorComp
        m_activeCount.deref();
    }
}

QString CExecutionEngineComp::GenerateId() const {
    return QStringLiteral("exec-") + QString::number(QDateTime::currentMSecsSinceEpoch());
}

} // namespace imtsentra
