// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// ImtSentra includes
#include <imtsentra/IScenarioGraph.h>

// Standard includes
#include <unordered_map>

namespace imtsentra
{

/**
 * \brief ACF component implementing IScenarioGraph
 *
 * Provides a directed graph implementation with support for
 * topological ordering, cycle detection, and JSON serialization.
 *
 * \ingroup imtsentra
 */
class CScenarioGraphComp:
        public icomp::CComponentBase,
        virtual public IScenarioGraph
{
public:
    typedef icomp::CComponentBase BaseClass;

    I_BEGIN_COMPONENT(CScenarioGraphComp)
        I_REGISTER_INTERFACE(IScenarioGraph);
    I_END_COMPONENT

    // reimplemented (imtsentra::IScenarioGraph)
    std::string GetId() const override;
    std::string GetName() const override;
    void SetName(const std::string& name) override;
    std::optional<std::string> GetDescription() const override;
    void SetDescription(const std::string& description) override;

    void AddNode(const ScenarioNode& node) override;
    void RemoveNode(const std::string& nodeId) override;
    void UpdateNode(const ScenarioNode& node) override;
    std::optional<ScenarioNode> GetNode(const std::string& nodeId) const override;
    std::vector<ScenarioNode> GetNodes() const override;

    void AddEdge(const ScenarioEdge& edge) override;
    void RemoveEdge(const std::string& edgeId) override;
    void UpdateEdge(const ScenarioEdge& edge) override;
    std::optional<ScenarioEdge> GetEdge(const std::string& edgeId) const override;
    std::vector<ScenarioEdge> GetEdges() const override;

    std::vector<ScenarioNode> GetSuccessors(const std::string& nodeId) const override;
    std::vector<ScenarioNode> GetPredecessors(const std::string& nodeId) const override;
    std::vector<ScenarioNode> GetRootNodes() const override;
    std::vector<ScenarioNode> GetTopologicalOrder() const override;

    bool IsValid() const override;
    bool HasCycles() const override;

    std::string ToJson() const override;
    bool FromJson(const std::string& json) override;

protected:
    // reimplemented (icomp::CComponentBase)
    void OnComponentCreated() override;

private:
    std::string m_id;
    std::string m_name;
    std::optional<std::string> m_description;
    std::unordered_map<std::string, ScenarioNode> m_nodes;
    std::unordered_map<std::string, ScenarioEdge> m_edges;

    bool DetectCyclesDFS(
        const std::string& nodeId,
        std::unordered_map<std::string, int>& visited
    ) const;
};

} // namespace imtsentra
