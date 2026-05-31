// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>
#include <istd/IPolymorphic.h>

// Qt includes
#include <QtCore/QString>

// Standard includes
#include <functional>

namespace sentragql
{

/**
 * \brief GraphQL service controller for Execution operations
 *
 * Handles:
 * - ExecuteScenario(scenarioId, config) → ExecutionId
 * - GetExecution(executionId) → Execution
 * - GetNodeResult(executionId, nodeId) → NodeExecutionResult
 * - StopExecution(executionId)
 * - RetryExecution(executionId, fromNodeId)
 * - Subscriptions: onExecutionProgress, onNodeCompleted
 *
 * \ingroup sentragql
 */
class CExecutionServiceControllerComp:
        public icomp::CComponentBase,
        virtual public istd::IPolymorphic
{
public:
    typedef icomp::CComponentBase BaseClass;

    I_BEGIN_COMPONENT(CExecutionServiceControllerComp)
        I_REGISTER_INTERFACE(istd::IPolymorphic);
    I_END_COMPONENT

    // Query resolvers
    QString GetExecution(const QString& executionId) const;
    QString GetNodeResult(const QString& executionId, const QString& nodeId) const;
    QString ListExecutions(const QString& scenarioId) const;

    // Mutation resolvers
    QString ExecuteScenario(const QString& scenarioId, const QString& configJson);
    bool StopExecution(const QString& executionId);
    QString RetryExecution(const QString& executionId, const QString& fromNodeId);

    // Subscription handlers
    using ProgressHandler = std::function<void(const QString& progressJson)>;
    void SubscribeExecutionProgress(const QString& executionId, ProgressHandler handler);
    void SubscribeNodeCompleted(const QString& executionId, ProgressHandler handler);
};

} // namespace sentragql
