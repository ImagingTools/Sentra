// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <cassert>
#include <iostream>
#include <string>
#include <imtsentra/CScenarioGraphComp.h>

using namespace imtsentra;

void testAddNode() {
    CScenarioGraphComp graph;
    graph.SetName("Test Scenario");

    ScenarioNode node;
    node.id = "node1";
    node.type = NT_OPEN_URL;
    node.label = "Open Home Page";
    node.posX = 100.0f;
    node.posY = 50.0f;

    graph.AddNode(node);

    auto retrieved = graph.GetNode("node1");
    assert(retrieved.has_value());
    assert(retrieved->id == "node1");
    assert(retrieved->type == NT_OPEN_URL);
    assert(retrieved->label == "Open Home Page");

    std::cout << "  [PASS] testAddNode" << std::endl;
}

void testRemoveNode() {
    CScenarioGraphComp graph;
    graph.SetName("Test");

    ScenarioNode n1{.id = "n1", .type = NT_OPEN_URL, .label = "Node 1"};
    ScenarioNode n2{.id = "n2", .type = NT_CLICK_ELEMENT, .label = "Node 2"};
    graph.AddNode(n1);
    graph.AddNode(n2);

    ScenarioEdge edge{.id = "e1", .sourceNodeId = "n1", .targetNodeId = "n2"};
    graph.AddEdge(edge);

    graph.RemoveNode("n1");

    assert(!graph.GetNode("n1").has_value());
    assert(graph.GetEdges().empty());  // Edge should be removed too

    std::cout << "  [PASS] testRemoveNode" << std::endl;
}

void testEdgeOperations() {
    CScenarioGraphComp graph;
    graph.SetName("Test");

    ScenarioNode n1{.id = "n1", .type = NT_OPEN_URL, .label = "Open"};
    ScenarioNode n2{.id = "n2", .type = NT_CLICK_ELEMENT, .label = "Click"};
    ScenarioNode n3{.id = "n3", .type = NT_TAKE_SCREENSHOT, .label = "Screenshot"};
    graph.AddNode(n1);
    graph.AddNode(n2);
    graph.AddNode(n3);

    ScenarioEdge e1{.id = "e1", .sourceNodeId = "n1", .targetNodeId = "n2"};
    ScenarioEdge e2{.id = "e2", .sourceNodeId = "n2", .targetNodeId = "n3"};
    graph.AddEdge(e1);
    graph.AddEdge(e2);

    assert(graph.GetEdges().size() == 2);

    auto successors = graph.GetSuccessors("n1");
    assert(successors.size() == 1);
    assert(successors[0].id == "n2");

    auto predecessors = graph.GetPredecessors("n3");
    assert(predecessors.size() == 1);
    assert(predecessors[0].id == "n2");

    std::cout << "  [PASS] testEdgeOperations" << std::endl;
}

void testTopologicalOrder() {
    CScenarioGraphComp graph;
    graph.SetName("Test");

    ScenarioNode n1{.id = "n1", .type = NT_OPEN_URL, .label = "Open"};
    ScenarioNode n2{.id = "n2", .type = NT_CLICK_ELEMENT, .label = "Click"};
    ScenarioNode n3{.id = "n3", .type = NT_TAKE_SCREENSHOT, .label = "Screenshot"};
    graph.AddNode(n1);
    graph.AddNode(n2);
    graph.AddNode(n3);

    graph.AddEdge(ScenarioEdge{.id = "e1", .sourceNodeId = "n1", .targetNodeId = "n2"});
    graph.AddEdge(ScenarioEdge{.id = "e2", .sourceNodeId = "n2", .targetNodeId = "n3"});

    auto order = graph.GetTopologicalOrder();
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
    graph.SetName("Test");

    ScenarioNode n1{.id = "n1", .type = NT_OPEN_URL, .label = "A"};
    ScenarioNode n2{.id = "n2", .type = NT_CLICK_ELEMENT, .label = "B"};
    graph.AddNode(n1);
    graph.AddNode(n2);

    graph.AddEdge(ScenarioEdge{.id = "e1", .sourceNodeId = "n1", .targetNodeId = "n2"});
    assert(!graph.HasCycles());

    graph.AddEdge(ScenarioEdge{.id = "e2", .sourceNodeId = "n2", .targetNodeId = "n1"});
    assert(graph.HasCycles());
    assert(!graph.IsValid());

    std::cout << "  [PASS] testCycleDetection" << std::endl;
}

void testRootNodes() {
    CScenarioGraphComp graph;
    graph.SetName("Test");

    ScenarioNode n1{.id = "n1", .type = NT_OPEN_URL, .label = "Root"};
    ScenarioNode n2{.id = "n2", .type = NT_CLICK_ELEMENT, .label = "Child"};
    graph.AddNode(n1);
    graph.AddNode(n2);

    graph.AddEdge(ScenarioEdge{.id = "e1", .sourceNodeId = "n1", .targetNodeId = "n2"});

    auto roots = graph.GetRootNodes();
    assert(roots.size() == 1);
    assert(roots[0].id == "n1");

    std::cout << "  [PASS] testRootNodes" << std::endl;
}

void testJsonSerialization() {
    CScenarioGraphComp graph;
    graph.SetName("Login Flow");
    graph.SetDescription("Tests the login process");

    ScenarioNode n1{.id = "n1", .type = NT_OPEN_URL, .label = "Open Login"};
    graph.AddNode(n1);

    auto json = graph.ToJson();
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
