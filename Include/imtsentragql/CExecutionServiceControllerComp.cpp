// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentragql/CExecutionServiceControllerComp.h>

namespace imtsentragql
{

QString CExecutionServiceControllerComp::GetExecution(const QString& executionId) const {
    // TODO: Get execution details
    return QStringLiteral("{}");
}

QString CExecutionServiceControllerComp::GetNodeResult(const QString& executionId, const QString& nodeId) const {
    // TODO: Get specific node result
    return QStringLiteral("{}");
}

QString CExecutionServiceControllerComp::ListExecutions(const QString& scenarioId) const {
    // TODO: List executions for scenario
    return QStringLiteral("[]");
}

QString CExecutionServiceControllerComp::ExecuteScenario(const QString& scenarioId, const QString& configJson) {
    // TODO: Start scenario execution
    return QStringLiteral("{}");
}

bool CExecutionServiceControllerComp::StopExecution(const QString& executionId) {
    // TODO: Stop execution
    return false;
}

QString CExecutionServiceControllerComp::RetryExecution(const QString& executionId, const QString& fromNodeId) {
    // TODO: Retry from node
    return QStringLiteral("{}");
}

void CExecutionServiceControllerComp::SubscribeExecutionProgress(const QString& executionId, ProgressHandler handler) {
    // TODO: Register subscription
}

void CExecutionServiceControllerComp::SubscribeNodeCompleted(const QString& executionId, ProgressHandler handler) {
    // TODO: Register subscription
}

} // namespace imtsentragql
