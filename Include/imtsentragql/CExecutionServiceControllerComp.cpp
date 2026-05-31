// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentragql/CExecutionServiceControllerComp.h>

namespace imtsentragql
{

std::string CExecutionServiceControllerComp::GetExecution(const std::string& executionId) const {
    // TODO: Get execution details
    return "{}";
}

std::string CExecutionServiceControllerComp::GetNodeResult(const std::string& executionId, const std::string& nodeId) const {
    // TODO: Get specific node result
    return "{}";
}

std::string CExecutionServiceControllerComp::ListExecutions(const std::string& scenarioId) const {
    // TODO: List executions for scenario
    return "[]";
}

std::string CExecutionServiceControllerComp::ExecuteScenario(const std::string& scenarioId, const std::string& configJson) {
    // TODO: Start scenario execution
    return "{}";
}

bool CExecutionServiceControllerComp::StopExecution(const std::string& executionId) {
    // TODO: Stop execution
    return false;
}

std::string CExecutionServiceControllerComp::RetryExecution(const std::string& executionId, const std::string& fromNodeId) {
    // TODO: Retry from node
    return "{}";
}

void CExecutionServiceControllerComp::SubscribeExecutionProgress(const std::string& executionId, ProgressHandler handler) {
    // TODO: Register subscription
}

void CExecutionServiceControllerComp::SubscribeNodeCompleted(const std::string& executionId, ProgressHandler handler) {
    // TODO: Register subscription
}

} // namespace imtsentragql
