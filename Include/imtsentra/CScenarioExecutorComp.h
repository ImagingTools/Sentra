// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// ImtSentra includes
#include <imtsentra/IScenarioExecutor.h>

// Qt includes
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QString>

namespace imtsentra
{

/**
 * \brief ACF component implementing IScenarioExecutor
 *
 * Executes scenario graphs by traversing nodes in topological order,
 * delegating each node to the appropriate INodeAction implementation.
 *
 * \ingroup imtsentra
 */
class CScenarioExecutorComp:
        public icomp::CComponentBase,
        virtual public IScenarioExecutor
{
public:
    typedef icomp::CComponentBase BaseClass;

    I_BEGIN_COMPONENT(CScenarioExecutorComp)
        I_REGISTER_INTERFACE(IScenarioExecutor);
    I_END_COMPONENT

    // reimplemented (imtsentra::IScenarioExecutor)
    QString Execute(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) override;

    void Stop(const QString& executionId) override;

    QString Retry(
        const QString& executionId,
        const QString& fromNodeId
    ) override;

    ExecutionStatus GetStatus(const QString& executionId) const override;
    QList<NodeExecutionResult> GetResults(const QString& executionId) const override;
    void OnProgress(ExecutionProgressCallback callback) override;

private:
    struct ExecutionState {
        QString id;
        ExecutionStatus status;
        std::shared_ptr<IScenarioGraph> graph;
        ExecutionConfig config;
        QList<NodeExecutionResult> results;
        bool stopRequested = false;
    };

    mutable QMutex m_mutex;
    QHash<QString, ExecutionState> m_executions;
    QList<ExecutionProgressCallback> m_callbacks;

    QString GenerateId() const;
    void NotifyProgress(const QString& executionId, const QString& nodeId,
                       ExecutionStatus status, float progress);
};

} // namespace imtsentra
