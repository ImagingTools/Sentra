// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// ImtSentra includes
#include <imtsentra/IScenarioGraph.h>

// Qt includes
#include <QtCore/QHash>
#include <QtCore/QString>

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
    QString GetId() const override;
    QString GetName() const override;
    void SetName(const QString& name) override;
    std::optional<QString> GetDescription() const override;
    void SetDescription(const QString& description) override;

    void AddNode(const ScenarioNode& node) override;
    void RemoveNode(const QString& nodeId) override;
    void UpdateNode(const ScenarioNode& node) override;
    std::optional<ScenarioNode> GetNode(const QString& nodeId) const override;
    QList<ScenarioNode> GetNodes() const override;

    void AddEdge(const ScenarioEdge& edge) override;
    void RemoveEdge(const QString& edgeId) override;
    void UpdateEdge(const ScenarioEdge& edge) override;
    std::optional<ScenarioEdge> GetEdge(const QString& edgeId) const override;
    QList<ScenarioEdge> GetEdges() const override;

    QList<ScenarioNode> GetSuccessors(const QString& nodeId) const override;
    QList<ScenarioNode> GetPredecessors(const QString& nodeId) const override;
    QList<ScenarioNode> GetRootNodes() const override;
    QList<ScenarioNode> GetTopologicalOrder() const override;

    bool IsValid() const override;
    bool HasCycles() const override;

    QString ToJson() const override;
    bool FromJson(const QString& json) override;

protected:
    // reimplemented (icomp::CComponentBase)
    void OnComponentCreated() override;

private:
    QString m_id;
    QString m_name;
    std::optional<QString> m_description;
    QHash<QString, ScenarioNode> m_nodes;
    QHash<QString, ScenarioEdge> m_edges;

    bool DetectCyclesDFS(
        const QString& nodeId,
        QHash<QString, int>& visited
    ) const;
};

} // namespace imtsentra
