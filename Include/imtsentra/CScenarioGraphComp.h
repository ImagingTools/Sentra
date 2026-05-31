#pragma once

#include "IScenarioGraph.h"
#include <unordered_map>

namespace imtsentra {

/**
 * @brief ACF Component implementing IScenarioGraph
 *
 * Provides a directed graph implementation with support for
 * topological ordering, cycle detection, and JSON serialization.
 */
class CScenarioGraphComp : public IScenarioGraph {
public:
    CScenarioGraphComp();
    ~CScenarioGraphComp() override;

    // IScenarioGraph implementation
    std::string getId() const override;
    std::string getName() const override;
    void setName(const std::string& name) override;
    std::optional<std::string> getDescription() const override;
    void setDescription(const std::string& description) override;

    void addNode(const ScenarioNode& node) override;
    void removeNode(const std::string& nodeId) override;
    void updateNode(const ScenarioNode& node) override;
    std::optional<ScenarioNode> getNode(const std::string& nodeId) const override;
    std::vector<ScenarioNode> getNodes() const override;

    void addEdge(const ScenarioEdge& edge) override;
    void removeEdge(const std::string& edgeId) override;
    void updateEdge(const ScenarioEdge& edge) override;
    std::optional<ScenarioEdge> getEdge(const std::string& edgeId) const override;
    std::vector<ScenarioEdge> getEdges() const override;

    std::vector<ScenarioNode> getSuccessors(const std::string& nodeId) const override;
    std::vector<ScenarioNode> getPredecessors(const std::string& nodeId) const override;
    std::vector<ScenarioNode> getRootNodes() const override;
    std::vector<ScenarioNode> getTopologicalOrder() const override;

    bool isValid() const override;
    bool hasCycles() const override;

    std::string toJson() const override;
    bool fromJson(const std::string& json) override;

private:
    std::string m_id;
    std::string m_name;
    std::optional<std::string> m_description;
    std::unordered_map<std::string, ScenarioNode> m_nodes;
    std::unordered_map<std::string, ScenarioEdge> m_edges;

    bool detectCyclesDFS(
        const std::string& nodeId,
        std::unordered_map<std::string, int>& visited
    ) const;
};

} // namespace imtsentra
