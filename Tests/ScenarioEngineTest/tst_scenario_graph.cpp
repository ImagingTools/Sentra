#include <cassert>
#include <iostream>
#include <string>
#include "imtsentra/CScenarioGraphComp.h"

using namespace imtsentra;

void testAddNode() {
    CScenarioGraphComp graph;
    graph.setName("Test Scenario");

    ScenarioNode node;
    node.id = "node1";
    node.type = NodeType::OpenUrl;
    node.label = "Open Home Page";
    node.posX = 100.0f;
    node.posY = 50.0f;

    graph.addNode(node);

    auto retrieved = graph.getNode("node1");
    assert(retrieved.has_value());
    assert(retrieved->id == "node1");
    assert(retrieved->type == NodeType::OpenUrl);
    assert(retrieved->label == "Open Home Page");

    std::cout << "  [PASS] testAddNode" << std::endl;
}

void testRemoveNode() {
    CScenarioGraphComp graph;
    graph.setName("Test");

    ScenarioNode n1{.id = "n1", .type = NodeType::OpenUrl, .label = "Node 1"};
    ScenarioNode n2{.id = "n2", .type = NodeType::ClickElement, .label = "Node 2"};
    graph.addNode(n1);
    graph.addNode(n2);

    ScenarioEdge edge{.id = "e1", .sourceNodeId = "n1", .targetNodeId = "n2"};
    graph.addEdge(edge);

    graph.removeNode("n1");

    assert(!graph.getNode("n1").has_value());
    assert(graph.getEdges().empty());  // Edge should be removed too

    std::cout << "  [PASS] testRemoveNode" << std::endl;
}

void testEdgeOperations() {
    CScenarioGraphComp graph;
    graph.setName("Test");

    ScenarioNode n1{.id = "n1", .type = NodeType::OpenUrl, .label = "Open"};
    ScenarioNode n2{.id = "n2", .type = NodeType::ClickElement, .label = "Click"};
    ScenarioNode n3{.id = "n3", .type = NodeType::TakeScreenshot, .label = "Screenshot"};
    graph.addNode(n1);
    graph.addNode(n2);
    graph.addNode(n3);

    ScenarioEdge e1{.id = "e1", .sourceNodeId = "n1", .targetNodeId = "n2"};
    ScenarioEdge e2{.id = "e2", .sourceNodeId = "n2", .targetNodeId = "n3"};
    graph.addEdge(e1);
    graph.addEdge(e2);

    assert(graph.getEdges().size() == 2);

    auto successors = graph.getSuccessors("n1");
    assert(successors.size() == 1);
    assert(successors[0].id == "n2");

    auto predecessors = graph.getPredecessors("n3");
    assert(predecessors.size() == 1);
    assert(predecessors[0].id == "n2");

    std::cout << "  [PASS] testEdgeOperations" << std::endl;
}

void testTopologicalOrder() {
    CScenarioGraphComp graph;
    graph.setName("Test");

    ScenarioNode n1{.id = "n1", .type = NodeType::OpenUrl, .label = "Open"};
    ScenarioNode n2{.id = "n2", .type = NodeType::ClickElement, .label = "Click"};
    ScenarioNode n3{.id = "n3", .type = NodeType::TakeScreenshot, .label = "Screenshot"};
    graph.addNode(n1);
    graph.addNode(n2);
    graph.addNode(n3);

    graph.addEdge(ScenarioEdge{.id = "e1", .sourceNodeId = "n1", .targetNodeId = "n2"});
    graph.addEdge(ScenarioEdge{.id = "e2", .sourceNodeId = "n2", .targetNodeId = "n3"});

    auto order = graph.getTopologicalOrder();
    assert(order.size() == 3);
    // n1 must come before n2, n2 before n3
    int idx_n1 = -1, idx_n2 = -1, idx_n3 = -1;
    for (int i = 0; i < (int)order.size(); i++) {
        if (order[i].id == "n1") idx_n1 = i;
        if (order[i].id == "n2") idx_n2 = i;
        if (order[i].id == "n3") idx_n3 = i;
    }
    assert(idx_n1 < idx_n2);
    assert(idx_n2 < idx_n3);

    std::cout << "  [PASS] testTopologicalOrder" << std::endl;
}

void testCycleDetection() {
    CScenarioGraphComp graph;
    graph.setName("Test");

    ScenarioNode n1{.id = "n1", .type = NodeType::OpenUrl, .label = "A"};
    ScenarioNode n2{.id = "n2", .type = NodeType::ClickElement, .label = "B"};
    graph.addNode(n1);
    graph.addNode(n2);

    graph.addEdge(ScenarioEdge{.id = "e1", .sourceNodeId = "n1", .targetNodeId = "n2"});
    assert(!graph.hasCycles());

    graph.addEdge(ScenarioEdge{.id = "e2", .sourceNodeId = "n2", .targetNodeId = "n1"});
    assert(graph.hasCycles());
    assert(!graph.isValid());

    std::cout << "  [PASS] testCycleDetection" << std::endl;
}

void testRootNodes() {
    CScenarioGraphComp graph;
    graph.setName("Test");

    ScenarioNode n1{.id = "n1", .type = NodeType::OpenUrl, .label = "Root"};
    ScenarioNode n2{.id = "n2", .type = NodeType::ClickElement, .label = "Child"};
    graph.addNode(n1);
    graph.addNode(n2);

    graph.addEdge(ScenarioEdge{.id = "e1", .sourceNodeId = "n1", .targetNodeId = "n2"});

    auto roots = graph.getRootNodes();
    assert(roots.size() == 1);
    assert(roots[0].id == "n1");

    std::cout << "  [PASS] testRootNodes" << std::endl;
}

void testJsonSerialization() {
    CScenarioGraphComp graph;
    graph.setName("Login Flow");
    graph.setDescription("Tests the login process");

    ScenarioNode n1{.id = "n1", .type = NodeType::OpenUrl, .label = "Open Login"};
    graph.addNode(n1);

    auto json = graph.toJson();
    assert(!json.empty());
    assert(json.find("Login Flow") != std::string::npos);
    assert(json.find("n1") != std::string::npos);

    std::cout << "  [PASS] testJsonSerialization" << std::endl;
}

int main() {
    std::cout << "=== ScenarioGraph Tests ===" << std::endl;

    testAddNode();
    testRemoveNode();
    testEdgeOperations();
    testTopologicalOrder();
    testCycleDetection();
    testRootNodes();
    testJsonSerialization();

    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}
