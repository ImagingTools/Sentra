#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace imtsentra {

/**
 * @brief Node type enumeration matching SDL schema
 */
enum class NodeType {
    OpenUrl,
    ClickElement,
    InputText,
    Wait,
    TakeScreenshot,
    CompareScreenshot,
    AssertText,
    AssertElement,
    AiValidation,
    ConditionalBranch,
    Loop
};

/**
 * @brief Represents a single node in a scenario graph
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
 * @brief Represents a directed edge between two scenario nodes
 */
struct ScenarioEdge {
    std::string id;
    std::string sourceNodeId;
    std::string targetNodeId;
    std::optional<std::string> condition;
    std::optional<std::string> label;
};

/**
 * @brief Interface for scenario graph operations
 *
 * Manages the directed graph structure of a test scenario.
 * Supports DAG with conditional branches and loops.
 */
class IScenarioGraph {
public:
    virtual ~IScenarioGraph() = default;

    // Graph identification
    virtual std::string getId() const = 0;
    virtual std::string getName() const = 0;
    virtual void setName(const std::string& name) = 0;
    virtual std::optional<std::string> getDescription() const = 0;
    virtual void setDescription(const std::string& description) = 0;

    // Node operations
    virtual void addNode(const ScenarioNode& node) = 0;
    virtual void removeNode(const std::string& nodeId) = 0;
    virtual void updateNode(const ScenarioNode& node) = 0;
    virtual std::optional<ScenarioNode> getNode(const std::string& nodeId) const = 0;
    virtual std::vector<ScenarioNode> getNodes() const = 0;

    // Edge operations
    virtual void addEdge(const ScenarioEdge& edge) = 0;
    virtual void removeEdge(const std::string& edgeId) = 0;
    virtual void updateEdge(const ScenarioEdge& edge) = 0;
    virtual std::optional<ScenarioEdge> getEdge(const std::string& edgeId) const = 0;
    virtual std::vector<ScenarioEdge> getEdges() const = 0;

    // Graph traversal
    virtual std::vector<ScenarioNode> getSuccessors(const std::string& nodeId) const = 0;
    virtual std::vector<ScenarioNode> getPredecessors(const std::string& nodeId) const = 0;
    virtual std::vector<ScenarioNode> getRootNodes() const = 0;
    virtual std::vector<ScenarioNode> getTopologicalOrder() const = 0;

    // Validation
    virtual bool isValid() const = 0;
    virtual bool hasCycles() const = 0;

    // Serialization
    virtual std::string toJson() const = 0;
    virtual bool fromJson(const std::string& json) = 0;
};

} // namespace imtsentra
