// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CScenarioGraphComp.h>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <random>
#include <chrono>

namespace imtsentra
{

void CScenarioGraphComp::OnComponentCreated() {
    BaseClass::OnComponentCreated();

    // Generate unique ID
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    m_id = "scenario-" + std::to_string(ms);
}

std::string CScenarioGraphComp::GetId() const { return m_id; }
std::string CScenarioGraphComp::GetName() const { return m_name; }
void CScenarioGraphComp::SetName(const std::string& name) { m_name = name; }
std::optional<std::string> CScenarioGraphComp::GetDescription() const { return m_description; }
void CScenarioGraphComp::SetDescription(const std::string& description) { m_description = description; }

void CScenarioGraphComp::AddNode(const ScenarioNode& node) {
    if (m_nodes.count(node.id)) {
        throw std::runtime_error("Node with id '" + node.id + "' already exists");
    }
    m_nodes[node.id] = node;
}

void CScenarioGraphComp::RemoveNode(const std::string& nodeId) {
    m_nodes.erase(nodeId);
    // Remove all edges connected to this node
    for (auto it = m_edges.begin(); it != m_edges.end();) {
        if (it->second.sourceNodeId == nodeId || it->second.targetNodeId == nodeId) {
            it = m_edges.erase(it);
        } else {
            ++it;
        }
    }
}

void CScenarioGraphComp::UpdateNode(const ScenarioNode& node) {
    if (!m_nodes.count(node.id)) {
        throw std::runtime_error("Node with id '" + node.id + "' not found");
    }
    m_nodes[node.id] = node;
}

std::optional<ScenarioNode> CScenarioGraphComp::GetNode(const std::string& nodeId) const {
    auto it = m_nodes.find(nodeId);
    if (it != m_nodes.end()) return it->second;
    return std::nullopt;
}

std::vector<ScenarioNode> CScenarioGraphComp::GetNodes() const {
    std::vector<ScenarioNode> nodes;
    nodes.reserve(m_nodes.size());
    for (const auto& [id, node] : m_nodes) {
        nodes.push_back(node);
    }
    return nodes;
}

void CScenarioGraphComp::AddEdge(const ScenarioEdge& edge) {
    if (m_edges.count(edge.id)) {
        throw std::runtime_error("Edge with id '" + edge.id + "' already exists");
    }
    if (!m_nodes.count(edge.sourceNodeId)) {
        throw std::runtime_error("Source node '" + edge.sourceNodeId + "' not found");
    }
    if (!m_nodes.count(edge.targetNodeId)) {
        throw std::runtime_error("Target node '" + edge.targetNodeId + "' not found");
    }
    m_edges[edge.id] = edge;
}

void CScenarioGraphComp::RemoveEdge(const std::string& edgeId) {
    m_edges.erase(edgeId);
}

void CScenarioGraphComp::UpdateEdge(const ScenarioEdge& edge) {
    if (!m_edges.count(edge.id)) {
        throw std::runtime_error("Edge with id '" + edge.id + "' not found");
    }
    m_edges[edge.id] = edge;
}

std::optional<ScenarioEdge> CScenarioGraphComp::GetEdge(const std::string& edgeId) const {
    auto it = m_edges.find(edgeId);
    if (it != m_edges.end()) return it->second;
    return std::nullopt;
}

std::vector<ScenarioEdge> CScenarioGraphComp::GetEdges() const {
    std::vector<ScenarioEdge> edges;
    edges.reserve(m_edges.size());
    for (const auto& [id, edge] : m_edges) {
        edges.push_back(edge);
    }
    return edges;
}

std::vector<ScenarioNode> CScenarioGraphComp::GetSuccessors(const std::string& nodeId) const {
    std::vector<ScenarioNode> successors;
    for (const auto& [id, edge] : m_edges) {
        if (edge.sourceNodeId == nodeId) {
            auto it = m_nodes.find(edge.targetNodeId);
            if (it != m_nodes.end()) {
                successors.push_back(it->second);
            }
        }
    }
    return successors;
}

std::vector<ScenarioNode> CScenarioGraphComp::GetPredecessors(const std::string& nodeId) const {
    std::vector<ScenarioNode> predecessors;
    for (const auto& [id, edge] : m_edges) {
        if (edge.targetNodeId == nodeId) {
            auto it = m_nodes.find(edge.sourceNodeId);
            if (it != m_nodes.end()) {
                predecessors.push_back(it->second);
            }
        }
    }
    return predecessors;
}

std::vector<ScenarioNode> CScenarioGraphComp::GetRootNodes() const {
    std::vector<ScenarioNode> roots;
    for (const auto& [id, node] : m_nodes) {
        bool hasIncoming = false;
        for (const auto& [eid, edge] : m_edges) {
            if (edge.targetNodeId == id) {
                hasIncoming = true;
                break;
            }
        }
        if (!hasIncoming) {
            roots.push_back(node);
        }
    }
    return roots;
}

std::vector<ScenarioNode> CScenarioGraphComp::GetTopologicalOrder() const {
    // Kahn's algorithm
    std::unordered_map<std::string, int> inDegree;
    for (const auto& [id, node] : m_nodes) {
        inDegree[id] = 0;
    }
    for (const auto& [id, edge] : m_edges) {
        inDegree[edge.targetNodeId]++;
    }

    std::vector<std::string> queue;
    for (const auto& [id, degree] : inDegree) {
        if (degree == 0) queue.push_back(id);
    }

    std::vector<ScenarioNode> result;
    while (!queue.empty()) {
        std::string current = queue.back();
        queue.pop_back();
        result.push_back(m_nodes.at(current));

        for (const auto& [id, edge] : m_edges) {
            if (edge.sourceNodeId == current) {
                inDegree[edge.targetNodeId]--;
                if (inDegree[edge.targetNodeId] == 0) {
                    queue.push_back(edge.targetNodeId);
                }
            }
        }
    }

    return result;
}

bool CScenarioGraphComp::IsValid() const {
    if (m_nodes.empty()) return false;
    if (HasCycles()) return false;
    // Check all edges reference existing nodes
    for (const auto& [id, edge] : m_edges) {
        if (!m_nodes.count(edge.sourceNodeId) || !m_nodes.count(edge.targetNodeId)) {
            return false;
        }
    }
    return true;
}

bool CScenarioGraphComp::HasCycles() const {
    std::unordered_map<std::string, int> visited;  // 0=unvisited, 1=in-stack, 2=done
    for (const auto& [id, node] : m_nodes) {
        visited[id] = 0;
    }
    for (const auto& [id, node] : m_nodes) {
        if (visited[id] == 0) {
            if (DetectCyclesDFS(id, visited)) return true;
        }
    }
    return false;
}

bool CScenarioGraphComp::DetectCyclesDFS(
    const std::string& nodeId,
    std::unordered_map<std::string, int>& visited
) const {
    visited[nodeId] = 1;
    for (const auto& [id, edge] : m_edges) {
        if (edge.sourceNodeId == nodeId) {
            if (visited[edge.targetNodeId] == 1) return true;
            if (visited[edge.targetNodeId] == 0) {
                if (DetectCyclesDFS(edge.targetNodeId, visited)) return true;
            }
        }
    }
    visited[nodeId] = 2;
    return false;
}

std::string CScenarioGraphComp::ToJson() const {
    // Simplified JSON serialization
    std::ostringstream ss;
    ss << "{";
    ss << "\"id\":\"" << m_id << "\",";
    ss << "\"name\":\"" << m_name << "\",";
    if (m_description) ss << "\"description\":\"" << *m_description << "\",";
    ss << "\"nodes\":[";
    bool first = true;
    for (const auto& [id, node] : m_nodes) {
        if (!first) ss << ",";
        ss << "{\"id\":\"" << node.id << "\","
           << "\"type\":" << static_cast<int>(node.type) << ","
           << "\"label\":\"" << node.label << "\","
           << "\"posX\":" << node.posX << ","
           << "\"posY\":" << node.posY << "}";
        first = false;
    }
    ss << "],\"edges\":[";
    first = true;
    for (const auto& [id, edge] : m_edges) {
        if (!first) ss << ",";
        ss << "{\"id\":\"" << edge.id << "\","
           << "\"sourceNodeId\":\"" << edge.sourceNodeId << "\","
           << "\"targetNodeId\":\"" << edge.targetNodeId << "\"}";
        first = false;
    }
    ss << "]}";
    return ss.str();
}

bool CScenarioGraphComp::FromJson(const std::string& json) {
    // TODO: Implement JSON deserialization
    // Will use Qt JSON or nlohmann/json
    return false;
}

} // namespace imtsentra
