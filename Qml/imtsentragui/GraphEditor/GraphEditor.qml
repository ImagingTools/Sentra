import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/**
 * GraphEditor — Visual scenario graph editor
 *
 * Provides a canvas for creating and editing test scenarios as directed graphs.
 * Supports drag & drop node placement, edge connections, zoom, pan, and undo/redo.
 */
Item {
    id: root

    property var scenario: null
    property real zoomLevel: 1.0
    property point panOffset: Qt.point(0, 0)

    signal nodeAdded(var nodeData)
    signal nodeRemoved(string nodeId)
    signal nodeSelected(string nodeId)
    signal edgeAdded(string sourceId, string targetId)
    signal edgeRemoved(string edgeId)
    signal scenarioChanged()

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Toolbar
        ToolBar {
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                spacing: 8

                ToolButton {
                    text: "▶ Run"
                    onClicked: executeScenario()
                }

                ToolButton {
                    text: "⏺ Record"
                    onClicked: startRecording()
                }

                ToolSeparator {}

                ToolButton {
                    text: "↩ Undo"
                    onClicked: undo()
                }

                ToolButton {
                    text: "↪ Redo"
                    onClicked: redo()
                }

                ToolSeparator {}

                ToolButton {
                    text: "🔍+"
                    onClicked: zoomIn()
                }

                ToolButton {
                    text: "🔍−"
                    onClicked: zoomOut()
                }

                ToolButton {
                    text: "⊡ Fit"
                    onClicked: fitToView()
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: scenario ? scenario.name : "No scenario"
                    font.bold: true
                }
            }
        }

        // Main content
        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: Qt.Horizontal

            // Node Palette (left panel)
            Rectangle {
                SplitView.preferredWidth: 200
                SplitView.minimumWidth: 150
                color: "#f5f5f5"

                ListView {
                    id: nodePalette
                    anchors.fill: parent
                    anchors.margins: 8

                    model: ListModel {
                        ListElement { name: "Open URL"; nodeType: "OpenUrl"; icon: "🌐" }
                        ListElement { name: "Click Element"; nodeType: "ClickElement"; icon: "👆" }
                        ListElement { name: "Input Text"; nodeType: "InputText"; icon: "⌨" }
                        ListElement { name: "Wait"; nodeType: "Wait"; icon: "⏳" }
                        ListElement { name: "Take Screenshot"; nodeType: "TakeScreenshot"; icon: "📸" }
                        ListElement { name: "Compare Screenshot"; nodeType: "CompareScreenshot"; icon: "🔍" }
                        ListElement { name: "Assert Text"; nodeType: "AssertText"; icon: "✓" }
                        ListElement { name: "Assert Element"; nodeType: "AssertElement"; icon: "☑" }
                        ListElement { name: "AI Validation"; nodeType: "AiValidation"; icon: "🤖" }
                        ListElement { name: "Condition"; nodeType: "ConditionalBranch"; icon: "⑂" }
                        ListElement { name: "Loop"; nodeType: "Loop"; icon: "🔄" }
                    }

                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 40
                        color: dragArea.containsMouse ? "#e0e0e0" : "transparent"
                        radius: 4

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 4
                            spacing: 8

                            Label {
                                text: model.icon
                                font.pixelSize: 16
                            }
                            Label {
                                text: model.name
                                Layout.fillWidth: true
                            }
                        }

                        MouseArea {
                            id: dragArea
                            anchors.fill: parent
                            hoverEnabled: true
                            drag.target: parent

                            onReleased: {
                                // TODO: Create node at drop position on canvas
                                root.nodeAdded({
                                    type: model.nodeType,
                                    label: model.name,
                                    posX: mouse.x + root.panOffset.x,
                                    posY: mouse.y + root.panOffset.y
                                })
                            }
                        }
                    }
                }
            }

            // Graph Canvas (center)
            Rectangle {
                SplitView.fillWidth: true
                color: "#ffffff"
                clip: true

                // TODO: Implement full graph canvas with:
                // - Node rendering (GraphNode.qml)
                // - Edge rendering (GraphEdge.qml with bezier curves)
                // - Drag & drop
                // - Selection
                // - Zoom & pan
                // - Minimap

                Label {
                    anchors.centerIn: parent
                    text: "Graph Canvas\n(Drop nodes here)"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20
                    color: "#999"
                    visible: !scenario || scenario.nodes.length === 0
                }
            }

            // Properties Panel (right)
            Rectangle {
                SplitView.preferredWidth: 250
                SplitView.minimumWidth: 200
                color: "#fafafa"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    Label {
                        text: "Node Properties"
                        font.bold: true
                        font.pixelSize: 14
                    }

                    // TODO: Dynamic property editor based on selected node type
                    Label {
                        text: "Select a node to edit properties"
                        color: "#666"
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                        visible: true  // TODO: Hide when node selected
                    }
                }
            }
        }
    }

    // Actions
    function executeScenario() {
        console.log("[Sentra] Executing scenario:", scenario ? scenario.name : "none")
    }

    function startRecording() {
        console.log("[Sentra] Starting recording mode")
    }

    function undo() { /* TODO */ }
    function redo() { /* TODO */ }
    function zoomIn() { zoomLevel = Math.min(zoomLevel * 1.2, 4.0) }
    function zoomOut() { zoomLevel = Math.max(zoomLevel / 1.2, 0.25) }
    function fitToView() { zoomLevel = 1.0; panOffset = Qt.point(0, 0) }
}
