// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// ImtSentra includes
#include <imtsentra/IScenarioExecutor.h>

// Standard includes
#include <mutex>
#include <unordered_map>

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
    std::string Execute(
        std::shared_ptr<IScenarioGraph> graph,
        const ExecutionConfig& config
    ) override;

    void Stop(const std::string& executionId) override;

    std::string Retry(
        const std::string& executionId,
        const std::string& fromNodeId
    ) override;

    ExecutionStatus GetStatus(const std::string& executionId) const override;
    std::vector<NodeExecutionResult> GetResults(const std::string& executionId) const override;
    void OnProgress(ExecutionProgressCallback callback) override;

private:
    struct ExecutionState {
        std::string id;
        ExecutionStatus status;
        std::shared_ptr<IScenarioGraph> graph;
        ExecutionConfig config;
        std::vector<NodeExecutionResult> results;
        bool stopRequested = false;
    };

    mutable std::mutex m_mutex;
    std::unordered_map<std::string, ExecutionState> m_executions;
    std::vector<ExecutionProgressCallback> m_callbacks;

    std::string GenerateId() const;
    void NotifyProgress(const std::string& executionId, const std::string& nodeId,
                       ExecutionStatus status, float progress);
};

} // namespace imtsentra
