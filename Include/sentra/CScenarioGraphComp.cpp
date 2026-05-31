// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <sentra/CScenarioGraphComp.h>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>

// Qt includes
#include <QtCore/QDateTime>

// Standard includes
#include <stdexcept>

namespace sentra
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

bool CScenarioGraphComp::Serialize(iser::IArchive& archive)
{
    istd::CChangeNotifier changeNotifier(archive.IsStoring() ? nullptr : this);

    bool retVal = true;

    static iser::CArchiveTag idTag("Id", "Scenario graph identifier", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(idTag);
    retVal = retVal && archive.Process(m_id);
    retVal = retVal && archive.EndTag(idTag);

    static iser::CArchiveTag nameTag("Name", "Scenario name", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(nameTag);
    retVal = retVal && archive.Process(m_name);
    retVal = retVal && archive.EndTag(nameTag);

    QString description = m_description.value_or(QString());
    static iser::CArchiveTag descriptionTag("Description", "Scenario description", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(descriptionTag);
    retVal = retVal && archive.Process(description);
    retVal = retVal && archive.EndTag(descriptionTag);
    if (!archive.IsStoring()){
        if (description.isEmpty()){
            m_description.reset();
        }
        else {
            m_description = description;
        }
    }

    // Nodes collection
    {
        QList<ScenarioNode> nodes = m_nodes.values();
        int count = nodes.size();
        if (!archive.IsStoring()){
            m_nodes.clear();
            count = 0;
        }

        iser::CArchiveTag nodesTag("Nodes", "Scenario nodes", iser::CArchiveTag::TT_MULTIPLE);
        iser::CArchiveTag nodeTag("Node", "Scenario node", iser::CArchiveTag::TT_GROUP, &nodesTag);

        retVal = retVal && archive.BeginMultiTag(nodesTag, nodeTag, count);

        for (int index = 0; index < count; ++index){
            retVal = retVal && archive.BeginTag(nodeTag);

            ScenarioNode node;
            if (archive.IsStoring()){
                node = nodes[index];
            }

            retVal = retVal && SerializeNode(archive, node);

            retVal = retVal && archive.EndTag(nodeTag);

            if (retVal && !archive.IsStoring()){
                m_nodes.insert(node.id, node);
            }
        }

        retVal = retVal && archive.EndTag(nodesTag);
    }

    // Edges collection
    {
        QList<ScenarioEdge> edges = m_edges.values();
        int count = edges.size();
        if (!archive.IsStoring()){
            m_edges.clear();
            count = 0;
        }

        iser::CArchiveTag edgesTag("Edges", "Scenario edges", iser::CArchiveTag::TT_MULTIPLE);
        iser::CArchiveTag edgeTag("Edge", "Scenario edge", iser::CArchiveTag::TT_GROUP, &edgesTag);

        retVal = retVal && archive.BeginMultiTag(edgesTag, edgeTag, count);

        for (int index = 0; index < count; ++index){
            retVal = retVal && archive.BeginTag(edgeTag);

            ScenarioEdge edge;
            if (archive.IsStoring()){
                edge = edges[index];
            }

            retVal = retVal && SerializeEdge(archive, edge);

            retVal = retVal && archive.EndTag(edgeTag);

            if (retVal && !archive.IsStoring()){
                m_edges.insert(edge.id, edge);
            }
        }

        retVal = retVal && archive.EndTag(edgesTag);
    }

    return retVal;
}

bool CScenarioGraphComp::SerializeNode(iser::IArchive& archive, ScenarioNode& node)
{
    bool retVal = true;

    static iser::CArchiveTag idTag("Id", "Node identifier", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(idTag);
    retVal = retVal && archive.Process(node.id);
    retVal = retVal && archive.EndTag(idTag);

    int type = node.type;
    static iser::CArchiveTag typeTag("Type", "Node type", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(typeTag);
    retVal = retVal && archive.Process(type);
    retVal = retVal && archive.EndTag(typeTag);
    if (!archive.IsStoring()){
        node.type = static_cast<NodeType>(type);
    }

    static iser::CArchiveTag labelTag("Label", "Node label", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(labelTag);
    retVal = retVal && archive.Process(node.label);
    retVal = retVal && archive.EndTag(labelTag);

    QString description = node.description.value_or(QString());
    static iser::CArchiveTag descriptionTag("Description", "Node description", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(descriptionTag);
    retVal = retVal && archive.Process(description);
    retVal = retVal && archive.EndTag(descriptionTag);
    if (!archive.IsStoring()){
        node.description = description.isEmpty() ? std::optional<QString>() : description;
    }

    QString config = node.config.value_or(QString());
    static iser::CArchiveTag configTag("Config", "Node JSON configuration", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(configTag);
    retVal = retVal && archive.Process(config);
    retVal = retVal && archive.EndTag(configTag);
    if (!archive.IsStoring()){
        node.config = config.isEmpty() ? std::optional<QString>() : config;
    }

    static iser::CArchiveTag posXTag("PosX", "Node X position", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(posXTag);
    retVal = retVal && archive.Process(node.posX);
    retVal = retVal && archive.EndTag(posXTag);

    static iser::CArchiveTag posYTag("PosY", "Node Y position", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(posYTag);
    retVal = retVal && archive.Process(node.posY);
    retVal = retVal && archive.EndTag(posYTag);

    return retVal;
}

bool CScenarioGraphComp::SerializeEdge(iser::IArchive& archive, ScenarioEdge& edge)
{
    bool retVal = true;

    static iser::CArchiveTag idTag("Id", "Edge identifier", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(idTag);
    retVal = retVal && archive.Process(edge.id);
    retVal = retVal && archive.EndTag(idTag);

    static iser::CArchiveTag sourceTag("SourceNodeId", "Source node identifier", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(sourceTag);
    retVal = retVal && archive.Process(edge.sourceNodeId);
    retVal = retVal && archive.EndTag(sourceTag);

    static iser::CArchiveTag targetTag("TargetNodeId", "Target node identifier", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(targetTag);
    retVal = retVal && archive.Process(edge.targetNodeId);
    retVal = retVal && archive.EndTag(targetTag);

    QString condition = edge.condition.value_or(QString());
    static iser::CArchiveTag conditionTag("Condition", "Edge condition", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(conditionTag);
    retVal = retVal && archive.Process(condition);
    retVal = retVal && archive.EndTag(conditionTag);
    if (!archive.IsStoring()){
        edge.condition = condition.isEmpty() ? std::optional<QString>() : condition;
    }

    QString label = edge.label.value_or(QString());
    static iser::CArchiveTag labelTag("Label", "Edge label", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(labelTag);
    retVal = retVal && archive.Process(label);
    retVal = retVal && archive.EndTag(labelTag);
    if (!archive.IsStoring()){
        edge.label = label.isEmpty() ? std::optional<QString>() : label;
    }

    return retVal;
}

} // namespace sentra
