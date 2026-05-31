import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/**
 * ExecutionTimeline — Horizontal timeline showing execution steps and their status
 *
 * Displays each node's execution result as a colored block on a timeline.
 * Clicking a step shows its details (screenshot, DOM, timing).
 */
Item {
    id: root

    property var execution: null
    property string selectedNodeId: ""

    signal stepSelected(string nodeId)
    signal stepDoubleClicked(string nodeId)

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Summary header
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            color: "#f8f8f8"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 8

                Label {
                    text: execution ? "Execution: " + execution.id : "No execution"
                    font.bold: true
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: execution ? statusText(execution.status) : ""
                    color: execution ? statusColor(execution.status) : "#666"
                    font.bold: true
                }

                Label {
                    text: execution ? execution.passedNodes + "/" + execution.totalNodes + " passed" : ""
                    color: "#666"
                }
            }
        }

        // Timeline visualization
        ScrollView {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            ScrollBar.horizontal.policy: ScrollBar.AsNeeded

            Row {
                spacing: 4
                padding: 8

                Repeater {
                    model: execution ? execution.nodeResults : []

                    Rectangle {
                        width: 60
                        height: 60
                        radius: 4
                        color: statusColor(modelData.status)
                        border.color: modelData.nodeId === root.selectedNodeId ? "#333" : "transparent"
                        border.width: 2

                        ToolTip.visible: mouseArea.containsMouse
                        ToolTip.text: modelData.nodeId + "\n" + modelData.status

                        Column {
                            anchors.centerIn: parent
                            spacing: 2

                            Label {
                                text: statusIcon(modelData.status)
                                font.pixelSize: 16
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            Label {
                                text: modelData.durationMs ? modelData.durationMs + "ms" : ""
                                font.pixelSize: 10
                                color: "#fff"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                root.selectedNodeId = modelData.nodeId
                                root.stepSelected(modelData.nodeId)
                            }
                            onDoubleClicked: {
                                root.stepDoubleClicked(modelData.nodeId)
                            }
                        }
                    }
                }
            }
        }

        // Step detail panel
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#fff"
            border.color: "#e0e0e0"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8

                Label {
                    text: selectedNodeId ? "Step: " + selectedNodeId : "Select a step to view details"
                    font.bold: true
                }

                // TODO: Show screenshot, DOM snapshot, timing details
                // for the selected step
            }
        }
    }

    // Helper functions
    function statusColor(status) {
        switch (status) {
            case "Passed": return "#4caf50"
            case "Failed": return "#f44336"
            case "Running": return "#2196f3"
            case "Skipped": return "#9e9e9e"
            case "TimedOut": return "#ff9800"
            default: return "#e0e0e0"
        }
    }

    function statusIcon(status) {
        switch (status) {
            case "Passed": return "✓"
            case "Failed": return "✗"
            case "Running": return "⟳"
            case "Skipped": return "−"
            case "TimedOut": return "⏱"
            default: return "○"
        }
    }

    function statusText(status) {
        return status || "Unknown"
    }
}
