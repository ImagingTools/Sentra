import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/**
 * AppMapViewer — Visualization of the application screen map as a graph
 *
 * Shows discovered screens as nodes and transitions as edges.
 * Hovering a screen shows its preview screenshot.
 * Indicates test coverage per screen.
 */
Item {
    id: root

    property var appMap: null
    property real zoomLevel: 1.0

    signal screenSelected(string screenId)
    signal transitionSelected(string transitionId)
    signal createScenarioRequested(string screenId)

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header
        ToolBar {
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                spacing: 8

                Label {
                    text: "Application Map"
                    font.bold: true
                    font.pixelSize: 14
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: appMap ? appMap.totalScreens + " screens discovered" : "No data"
                    color: "#666"
                }

                Label {
                    text: appMap ? "Coverage: " + coverageText() : ""
                    color: coverageColor()
                    font.bold: true
                }
            }
        }

        // Map canvas
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#fafafa"

            // TODO: Implement force-directed graph layout for screens
            // Each screen rendered as a card with:
            // - Title
            // - URL
            // - Mini screenshot preview
            // - Coverage indicator (green/red border)
            //
            // Edges show transitions with action labels

            Label {
                anchors.centerIn: parent
                text: appMap && appMap.screens.length > 0
                    ? ""
                    : "Run scenarios to build the application map"
                color: "#999"
                font.pixelSize: 16
            }

            // Screen nodes (simplified placeholder)
            Repeater {
                model: appMap ? appMap.screens : []

                Rectangle {
                    x: 50 + index * 180
                    y: 50 + (index % 3) * 120
                    width: 160
                    height: 100
                    radius: 8
                    color: "#ffffff"
                    border.color: modelData.visitCount > 0 ? "#4caf50" : "#f44336"
                    border.width: 2

                    Column {
                        anchors.centerIn: parent
                        spacing: 4

                        Label {
                            text: modelData.title || "Untitled"
                            font.bold: true
                            anchors.horizontalCenter: parent.horizontalCenter
                            elide: Text.ElideRight
                            width: 140
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Label {
                            text: modelData.url || ""
                            font.pixelSize: 10
                            color: "#666"
                            anchors.horizontalCenter: parent.horizontalCenter
                            elide: Text.ElideMiddle
                            width: 140
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Label {
                            text: modelData.visitCount + " visits"
                            font.pixelSize: 10
                            color: "#999"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: root.screenSelected(modelData.id)
                        onDoubleClicked: root.createScenarioRequested(modelData.id)
                    }
                }
            }
        }
    }

    function coverageText() {
        if (!appMap) return ""
        var pct = (appMap.coveredScreens / Math.max(appMap.totalScreens, 1) * 100).toFixed(0)
        return pct + "%"
    }

    function coverageColor() {
        if (!appMap) return "#666"
        var pct = appMap.coveredScreens / Math.max(appMap.totalScreens, 1)
        if (pct >= 0.8) return "#4caf50"
        if (pct >= 0.5) return "#ff9800"
        return "#f44336"
    }
}
