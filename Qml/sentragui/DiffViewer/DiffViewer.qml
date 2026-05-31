import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/**
 * DiffViewer — Visual comparison viewer for baseline vs actual screenshots
 *
 * Supports multiple view modes:
 * - Side by side
 * - Overlay with transparency slider
 * - Slider (swipe between images)
 * - Highlight differences
 */
Item {
    id: root

    property string baselinePath: ""
    property string actualPath: ""
    property string diffPath: ""
    property real diffPercentage: 0.0
    property string viewMode: "sideBySide"  // sideBySide, overlay, slider, highlight

    signal accepted()
    signal rejected()
    signal ignoreRegionAdded(rect region)

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Controls bar
        ToolBar {
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                spacing: 8

                ButtonGroup {
                    id: viewModeGroup
                }

                ToolButton {
                    text: "Side by Side"
                    checkable: true
                    checked: viewMode === "sideBySide"
                    ButtonGroup.group: viewModeGroup
                    onClicked: viewMode = "sideBySide"
                }

                ToolButton {
                    text: "Overlay"
                    checkable: true
                    checked: viewMode === "overlay"
                    ButtonGroup.group: viewModeGroup
                    onClicked: viewMode = "overlay"
                }

                ToolButton {
                    text: "Slider"
                    checkable: true
                    checked: viewMode === "slider"
                    ButtonGroup.group: viewModeGroup
                    onClicked: viewMode = "slider"
                }

                ToolButton {
                    text: "Highlight"
                    checkable: true
                    checked: viewMode === "highlight"
                    ButtonGroup.group: viewModeGroup
                    onClicked: viewMode = "highlight"
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: "Diff: " + diffPercentage.toFixed(2) + "%"
                    color: diffPercentage > 5 ? "#f44336" : diffPercentage > 1 ? "#ff9800" : "#4caf50"
                    font.bold: true
                }
            }
        }

        // Image comparison area
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Side by side view
            SplitView {
                anchors.fill: parent
                visible: viewMode === "sideBySide"
                orientation: Qt.Horizontal

                Image {
                    source: baselinePath ? "file://" + baselinePath : ""
                    fillMode: Image.PreserveAspectFit
                    SplitView.fillWidth: true

                    Label {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.margins: 8
                        text: "Baseline"
                        color: "#4caf50"
                        font.bold: true
                        background: Rectangle { color: "#ffffff80"; radius: 4 }
                        padding: 4
                    }
                }

                Image {
                    source: actualPath ? "file://" + actualPath : ""
                    fillMode: Image.PreserveAspectFit
                    SplitView.fillWidth: true

                    Label {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.margins: 8
                        text: "Actual"
                        color: "#2196f3"
                        font.bold: true
                        background: Rectangle { color: "#ffffff80"; radius: 4 }
                        padding: 4
                    }
                }
            }

            // Highlight view (diff overlay)
            Image {
                anchors.fill: parent
                source: diffPath ? "file://" + diffPath : ""
                fillMode: Image.PreserveAspectFit
                visible: viewMode === "highlight"
            }

            // TODO: Overlay view with opacity slider
            // TODO: Slider swipe view
        }

        // Decision buttons
        ToolBar {
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                spacing: 12

                Item { Layout.fillWidth: true }

                Button {
                    text: "✓ Accept (Update Baseline)"
                    palette.button: "#4caf50"
                    palette.buttonText: "#ffffff"
                    onClicked: root.accepted()
                }

                Button {
                    text: "✗ Reject (Mark as Failed)"
                    palette.button: "#f44336"
                    palette.buttonText: "#ffffff"
                    onClicked: root.rejected()
                }

                Button {
                    text: "⊞ Ignore Region"
                    onClicked: {
                        // TODO: Enable region selection mode
                    }
                }

                Item { Layout.fillWidth: true }
            }
        }
    }
}
