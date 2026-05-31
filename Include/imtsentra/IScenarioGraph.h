// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <istd/istd.h>
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Qt includes
#include <QtCore/QString>
#include <QtCore/QList>

// Standard includes
#include <memory>
#include <optional>

namespace imtsentra
{

/**
 * \brief Node type enumeration matching SDL schema
 *
 * \ingroup imtsentra
 */
enum NodeType
{
    NT_OPEN_URL,
    NT_CLICK_ELEMENT,
    NT_INPUT_TEXT,
    NT_WAIT,
    NT_TAKE_SCREENSHOT,
    NT_COMPARE_SCREENSHOT,
    NT_ASSERT_TEXT,
    NT_ASSERT_ELEMENT,
    NT_AI_VALIDATION,
    NT_CONDITIONAL_BRANCH,
    NT_LOOP
};
I_DECLARE_ENUM(NodeType,
            NT_OPEN_URL,
            NT_CLICK_ELEMENT,
            NT_INPUT_TEXT,
            NT_WAIT,
            NT_TAKE_SCREENSHOT,
            NT_COMPARE_SCREENSHOT,
            NT_ASSERT_TEXT,
            NT_ASSERT_ELEMENT,
            NT_AI_VALIDATION,
            NT_CONDITIONAL_BRANCH,
            NT_LOOP);

/**
 * \brief Represents a single node in a scenario graph
 */
struct ScenarioNode {
    QString id;
    NodeType type;
    QString label;
    std::optional<QString> description;
    std::optional<QString> config;  // JSON config
    float posX = 0.0f;
    float posY = 0.0f;
};

/**
 * \brief Represents a directed edge between two scenario nodes
 */
struct ScenarioEdge {
    QString id;
    QString sourceNodeId;
    QString targetNodeId;
    std::optional<QString> condition;
    std::optional<QString> label;
};

/**
 * \brief Interface for scenario graph operations
 *
 * Manages the directed graph structure of a test scenario.
 * Supports DAG with conditional branches and loops.
 *
 * \ingroup imtsentra
 */
class IScenarioGraph: virtual public istd::IPolymorphic
{
public:
    // Graph identification
    virtual QString GetId() const = 0;
    virtual QString GetName() const = 0;
    virtual void SetName(const QString& name) = 0;
    virtual std::optional<QString> GetDescription() const = 0;
    virtual void SetDescription(const QString& description) = 0;

    // Node operations
    virtual void AddNode(const ScenarioNode& node) = 0;
    virtual void RemoveNode(const QString& nodeId) = 0;
    virtual void UpdateNode(const ScenarioNode& node) = 0;
    virtual std::optional<ScenarioNode> GetNode(const QString& nodeId) const = 0;
    virtual QList<ScenarioNode> GetNodes() const = 0;

    // Edge operations
    virtual void AddEdge(const ScenarioEdge& edge) = 0;
    virtual void RemoveEdge(const QString& edgeId) = 0;
    virtual void UpdateEdge(const ScenarioEdge& edge) = 0;
    virtual std::optional<ScenarioEdge> GetEdge(const QString& edgeId) const = 0;
    virtual QList<ScenarioEdge> GetEdges() const = 0;

    // Graph traversal
    virtual QList<ScenarioNode> GetSuccessors(const QString& nodeId) const = 0;
    virtual QList<ScenarioNode> GetPredecessors(const QString& nodeId) const = 0;
    virtual QList<ScenarioNode> GetRootNodes() const = 0;
    virtual QList<ScenarioNode> GetTopologicalOrder() const = 0;

    // Validation
    virtual bool IsValid() const = 0;
    virtual bool HasCycles() const = 0;

    // Serialization
    virtual QString ToJson() const = 0;
    virtual bool FromJson(const QString& json) = 0;
};

typedef istd::TUniqueInterfacePtr<IScenarioGraph> IScenarioGraphUniquePtr;

} // namespace imtsentra
