// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// ImtSentra includes
#include <imtsentra/IExecutionEngine.h>

// Qt includes
#include <QtCore/QAtomicInt>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QQueue>
#include <QtCore/QString>
#include <QtCore/QWaitCondition>

// Standard includes
#include <memory>

QT_BEGIN_NAMESPACE
class QThread;
QT_END_NAMESPACE

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
    QString QueueExecution(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) override;

    QList<QString> QueueParallelExecution(
        const QList<std::shared_ptr<IScenarioGraph>>& graphs,
        const ExecutionConfig& config
    ) override;

    void CancelExecution(const QString& executionId) override;
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
        QString id;
        std::shared_ptr<IScenarioGraph> graph;
        ExecutionConfig config;
    };

    EngineConfig m_config;
    mutable QMutex m_mutex;
    QQueue<QueuedExecution> m_queue;
    QList<QThread*> m_workers;
    QWaitCondition m_cv;
    QAtomicInt m_running{0};
    QAtomicInt m_activeCount{0};

    void WorkerLoop();
    QString GenerateId() const;
};

} // namespace imtsentra
