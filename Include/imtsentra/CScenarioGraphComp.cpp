// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CScenarioGraphComp.h>

// Qt includes
#include <QtCore/QDateTime>

// Standard includes
#include <stdexcept>

namespace imtsentra
{

void CScenarioGraphComp::OnComponentCreated() {
    BaseClass::OnComponentCreated();

    // Generate unique ID
    m_id = QStringLiteral("scenario-") + QString::number(QDateTime::currentMSecsSinceEpoch());
}

QString CScenarioGraphComp::GetId() const { return m_id; }
QString CScenarioGraphComp::GetName() const { return m_name; }
void CScenarioGraphComp::SetName(const QString& name) { m_name = name; }
std::optional<QString> CScenarioGraphComp::GetDescription() const { return m_description; }
void CScenarioGraphComp::SetDescription(const QString& description) { m_description = description; }

void CScenarioGraphComp::AddNode(const ScenarioNode& node) {
    if (m_nodes.contains(node.id)) {
        throw std::runtime_error(
            QStringLiteral("Node with id '%1' already exists").arg(node.id).toStdString());
    }
    m_nodes[node.id] = node;
}

void CScenarioGraphComp::RemoveNode(const QString& nodeId) {
    m_nodes.remove(nodeId);
    // Remove all edges connected to this node
    for (auto it = m_edges.begin(); it != m_edges.end();) {
        if (it.value().sourceNodeId == nodeId || it.value().targetNodeId == nodeId) {
            it = m_edges.erase(it);
        } else {
            ++it;
        }
    }
}

void CScenarioGraphComp::UpdateNode(const ScenarioNode& node) {
    if (!m_nodes.contains(node.id)) {
        throw std::runtime_error(
            QStringLiteral("Node with id '%1' not found").arg(node.id).toStdString());
    }
    m_nodes[node.id] = node;
}

std::optional<ScenarioNode> CScenarioGraphComp::GetNode(const QString& nodeId) const {
    auto it = m_nodes.find(nodeId);
    if (it != m_nodes.end()) return it.value();
    return std::nullopt;
}

QList<ScenarioNode> CScenarioGraphComp::GetNodes() const {
    QList<ScenarioNode> nodes;
    nodes.reserve(m_nodes.size());
    for (const auto& node : m_nodes) {
        nodes.push_back(node);
    }
    return nodes;
}

void CScenarioGraphComp::AddEdge(const ScenarioEdge& edge) {
    if (m_edges.contains(edge.id)) {
        throw std::runtime_error(
            QStringLiteral("Edge with id '%1' already exists").arg(edge.id).toStdString());
    }
    if (!m_nodes.contains(edge.sourceNodeId)) {
        throw std::runtime_error(
            QStringLiteral("Source node '%1' not found").arg(edge.sourceNodeId).toStdString());
    }
    if (!m_nodes.contains(edge.targetNodeId)) {
        throw std::runtime_error(
            QStringLiteral("Target node '%1' not found").arg(edge.targetNodeId).toStdString());
    }
    m_edges[edge.id] = edge;
}

void CScenarioGraphComp::RemoveEdge(const QString& edgeId) {
    m_edges.remove(edgeId);
}

void CScenarioGraphComp::UpdateEdge(const ScenarioEdge& edge) {
    if (!m_edges.contains(edge.id)) {
        throw std::runtime_error(
            QStringLiteral("Edge with id '%1' not found").arg(edge.id).toStdString());
    }
    m_edges[edge.id] = edge;
}

std::optional<ScenarioEdge> CScenarioGraphComp::GetEdge(const QString& edgeId) const {
    auto it = m_edges.find(edgeId);
    if (it != m_edges.end()) return it.value();
    return std::nullopt;
}

QList<ScenarioEdge> CScenarioGraphComp::GetEdges() const {
    QList<ScenarioEdge> edges;
    edges.reserve(m_edges.size());
    for (const auto& edge : m_edges) {
        edges.push_back(edge);
    }
    return edges;
}

QList<ScenarioNode> CScenarioGraphComp::GetSuccessors(const QString& nodeId) const {
    QList<ScenarioNode> successors;
    for (const auto& edge : m_edges) {
        if (edge.sourceNodeId == nodeId) {
            auto it = m_nodes.find(edge.targetNodeId);
            if (it != m_nodes.end()) {
                successors.push_back(it.value());
            }
        }
    }
    return successors;
}

QList<ScenarioNode> CScenarioGraphComp::GetPredecessors(const QString& nodeId) const {
    QList<ScenarioNode> predecessors;
    for (const auto& edge : m_edges) {
        if (edge.targetNodeId == nodeId) {
            auto it = m_nodes.find(edge.sourceNodeId);
            if (it != m_nodes.end()) {
                predecessors.push_back(it.value());
            }
        }
    }
    return predecessors;
}

QList<ScenarioNode> CScenarioGraphComp::GetRootNodes() const {
    QList<ScenarioNode> roots;
    for (auto nodeIt = m_nodes.cbegin(); nodeIt != m_nodes.cend(); ++nodeIt) {
        bool hasIncoming = false;
        for (const auto& edge : m_edges) {
            if (edge.targetNodeId == nodeIt.key()) {
                hasIncoming = true;
                break;
            }
        }
        if (!hasIncoming) {
            roots.push_back(nodeIt.value());
        }
    }
    return roots;
}

QList<ScenarioNode> CScenarioGraphComp::GetTopologicalOrder() const {
    // Kahn's algorithm
    QHash<QString, int> inDegree;
    for (auto it = m_nodes.cbegin(); it != m_nodes.cend(); ++it) {
        inDegree[it.key()] = 0;
    }
    for (const auto& edge : m_edges) {
        inDegree[edge.targetNodeId]++;
    }

    QList<QString> queue;
    for (auto it = inDegree.cbegin(); it != inDegree.cend(); ++it) {
        if (it.value() == 0) queue.push_back(it.key());
    }

    QList<ScenarioNode> result;
    while (!queue.isEmpty()) {
        QString current = queue.back();
        queue.pop_back();
        result.push_back(m_nodes.value(current));

        for (const auto& edge : m_edges) {
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
    if (m_nodes.isEmpty()) return false;
    if (HasCycles()) return false;
    // Check all edges reference existing nodes
    for (const auto& edge : m_edges) {
        if (!m_nodes.contains(edge.sourceNodeId) || !m_nodes.contains(edge.targetNodeId)) {
            return false;
        }
    }
    return true;
}

bool CScenarioGraphComp::HasCycles() const {
    QHash<QString, int> visited;  // 0=unvisited, 1=in-stack, 2=done
    for (auto it = m_nodes.cbegin(); it != m_nodes.cend(); ++it) {
        visited[it.key()] = 0;
    }
    for (auto it = m_nodes.cbegin(); it != m_nodes.cend(); ++it) {
        if (visited[it.key()] == 0) {
            if (DetectCyclesDFS(it.key(), visited)) return true;
        }
    }
    return false;
}

bool CScenarioGraphComp::DetectCyclesDFS(
    const QString& nodeId,
    QHash<QString, int>& visited
) const {
    visited[nodeId] = 1;
    for (const auto& edge : m_edges) {
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

QString CScenarioGraphComp::ToJson() const {
    // Simplified JSON serialization
    QString json = QStringLiteral("{");
    json += QStringLiteral("\"id\":\"") + m_id + QStringLiteral("\",");
    json += QStringLiteral("\"name\":\"") + m_name + QStringLiteral("\",");
    if (m_description) json += QStringLiteral("\"description\":\"") + *m_description + QStringLiteral("\",");
    json += QStringLiteral("\"nodes\":[");
    bool first = true;
    for (const auto& node : m_nodes) {
        if (!first) json += QLatin1Char(',');
        json += QStringLiteral("{\"id\":\"") + node.id + QStringLiteral("\",")
              + QStringLiteral("\"type\":") + QString::number(static_cast<int>(node.type)) + QStringLiteral(",")
              + QStringLiteral("\"label\":\"") + node.label + QStringLiteral("\",")
              + QStringLiteral("\"posX\":") + QString::number(node.posX) + QStringLiteral(",")
              + QStringLiteral("\"posY\":") + QString::number(node.posY) + QStringLiteral("}");
        first = false;
    }
    json += QStringLiteral("],\"edges\":[");
    first = true;
    for (const auto& edge : m_edges) {
        if (!first) json += QLatin1Char(',');
        json += QStringLiteral("{\"id\":\"") + edge.id + QStringLiteral("\",")
              + QStringLiteral("\"sourceNodeId\":\"") + edge.sourceNodeId + QStringLiteral("\",")
              + QStringLiteral("\"targetNodeId\":\"") + edge.targetNodeId + QStringLiteral("\"}");
        first = false;
    }
    json += QStringLiteral("]}");
    return json;
}

bool CScenarioGraphComp::FromJson(const QString& json) {
    // TODO: Implement JSON deserialization
    // Will use Qt JSON (QJsonDocument)
    return false;
}

} // namespace imtsentra
