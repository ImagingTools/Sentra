// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <istd/istd.h>
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Standard includes
#include <string>
#include <vector>
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
    std::string id;
    NodeType type;
    std::string label;
    std::optional<std::string> description;
    std::optional<std::string> config;  // JSON config
    float posX = 0.0f;
    float posY = 0.0f;
};

/**
 * \brief Represents a directed edge between two scenario nodes
 */
struct ScenarioEdge {
    std::string id;
    std::string sourceNodeId;
    std::string targetNodeId;
    std::optional<std::string> condition;
    std::optional<std::string> label;
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
    virtual std::string GetId() const = 0;
    virtual std::string GetName() const = 0;
    virtual void SetName(const std::string& name) = 0;
    virtual std::optional<std::string> GetDescription() const = 0;
    virtual void SetDescription(const std::string& description) = 0;

    // Node operations
    virtual void AddNode(const ScenarioNode& node) = 0;
    virtual void RemoveNode(const std::string& nodeId) = 0;
    virtual void UpdateNode(const ScenarioNode& node) = 0;
    virtual std::optional<ScenarioNode> GetNode(const std::string& nodeId) const = 0;
    virtual std::vector<ScenarioNode> GetNodes() const = 0;

    // Edge operations
    virtual void AddEdge(const ScenarioEdge& edge) = 0;
    virtual void RemoveEdge(const std::string& edgeId) = 0;
    virtual void UpdateEdge(const ScenarioEdge& edge) = 0;
    virtual std::optional<ScenarioEdge> GetEdge(const std::string& edgeId) const = 0;
    virtual std::vector<ScenarioEdge> GetEdges() const = 0;

    // Graph traversal
    virtual std::vector<ScenarioNode> GetSuccessors(const std::string& nodeId) const = 0;
    virtual std::vector<ScenarioNode> GetPredecessors(const std::string& nodeId) const = 0;
    virtual std::vector<ScenarioNode> GetRootNodes() const = 0;
    virtual std::vector<ScenarioNode> GetTopologicalOrder() const = 0;

    // Validation
    virtual bool IsValid() const = 0;
    virtual bool HasCycles() const = 0;

    // Serialization
    virtual std::string ToJson() const = 0;
    virtual bool FromJson(const std::string& json) = 0;
};

typedef istd::TUniqueInterfacePtr<IScenarioGraph> IScenarioGraphUniquePtr;

} // namespace imtsentra
