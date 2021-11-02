import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts
import Fannn

SpacedGridDelegate {
    required property var curves
    required property Item everythingItem

    required property int index
    required property var curve

    id: top

    Dialog {
        id: curveEditorDlg
        parent: everythingItem
        width: Math.min(700, parent.width)
        height: Math.min(700, parent.height)
        x: (parent.width-width)/2
        y: (parent.height-height)/2
        rightPadding: 40
        leftPadding: 40
        modal: true
        standardButtons: Dialog.NoButton

        Item {
            id: dlgName
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: minMaxLayout.left
            anchors.leftMargin: 4
            height: dlgNameField.height
            SSoftValidatedField {
                id: dlgNameField
                validator: IdentifierValidator {}
                anchors.left: parent.left
                anchors.top: parent.top
                width: Math.min(Math.max(150, contentWidth + rightPadding + leftPadding + 2), parent.width)
                text: curve.name

                function set(value) {
                    if (!text)
                        return "name cannot be empty"
                    if (!curve.rename(text))
                        return "name already used"
                    return ""
                }
            }
        }
        Row {
            id: minMaxLayout
            anchors.right: parent.right
            anchors.bottom: dlgName.bottom
            anchors.rightMargin: 6
            spacing: 5
            property int fieldWidth: 45
            property int spaceAboveLine: 2
            IntValidator  {
                id: validator
                bottom: -9999
                top: 9999
            }
            Label {
                text: "x:"
                height: parent.height
                leftPadding: 14
                horizontalAlignment: TextInput.AlignRight
                verticalAlignment: TextInput.AlignVCenter
            }
            STextField {
                text: "0"
                width: minMaxLayout.fieldWidth
                spaceAboveLine: minMaxLayout.spaceAboveLine
                leftPadding: 1
                rightPadding: 1
                horizontalAlignment: TextInput.AlignHCenter
                validator: validator
            }
            Label {
                text: "-"
                height: parent.height
                verticalAlignment: TextInput.AlignVCenter
            }
            STextField {
                text: "100"
                width: minMaxLayout.fieldWidth
                spaceAboveLine: minMaxLayout.spaceAboveLine
                leftPadding: 1
                rightPadding: 1
                horizontalAlignment: TextInput.AlignHCenter
                validator: validator
            }
            Label {
                text: "y:"
                height: parent.height
                leftPadding: 14
                horizontalAlignment: TextInput.AlignRight
                verticalAlignment: TextInput.AlignVCenter
            }
            STextField {
                text: "0"
                width: minMaxLayout.fieldWidth
                spaceAboveLine: minMaxLayout.spaceAboveLine
                leftPadding: 1
                rightPadding: 1
                horizontalAlignment: TextInput.AlignHCenter
                validator: validator
            }
            Label {
                text: "-"
                height: parent.height
                verticalAlignment: TextInput.AlignVCenter
            }
            STextField {
                text: "100"
                width: minMaxLayout.fieldWidth
                spaceAboveLine: minMaxLayout.spaceAboveLine
                leftPadding: 1
                rightPadding: 1
                horizontalAlignment: TextInput.AlignHCenter
                validator: validator
            }
        }

        CurveChartView {
            id: dlgChart
            curve: top.curve
            anchors.top: dlgName.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left

            Repeater {
                model: curve

                Rectangle {
                    id: indicator
                    required property int index
                    required property var _point

                    property real chartPointX: (_point.x * dlgChart.plotArea.width / 100) + dlgChart.plotArea.x //todo: less magic
                    property real chartPointY: dlgChart.plotArea.height - (_point.y * dlgChart.plotArea.height / 100) + dlgChart.plotArea.y //todo: less magic

                    radius: 9
                    width: radius * 2
                    height: radius * 2
                    color: "orange"//todo
                    x: chartPointX - radius
                    y: chartPointY - radius

                    Item {
                        id: handle
                        parent: dlgChart
                        width: indicator.width * 1.15
                        height: indicator.height * 1.15

                        Binding {
                            target: handle
                            property: "x"
                            value: indicator.x
                            when: !mouseArea.drag.active
                        }

                        Binding {
                            target: handle
                            property: "y"
                            value: indicator.y
                            when: !mouseArea.drag.active
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            drag.target: handle
                            drag.threshold: 0
                            acceptedButtons: Qt.LeftButton | Qt.RightButton

                            drag.onActiveChanged: {
                                if (drag.active)
                                    curve.beginMovePoint(index)
                                else
                                    curve.endMovePoint()
                            }

                            onPressed: function(mouse) {
                                if (mouse.button === Qt.RightButton && curve.rowCount() > 1) {
                                    curve.removePoint(index)
                                }
                            }

                            onPositionChanged: {
                                if (index === curve.movingPointIndex) {
                                    let point = dlgChart.mapToValue(Qt.point(handle.x + width / 2, handle.y + height / 2), dlgChart.series(0))
                                    curve.movePoint(point)
                                }
                            }
                        }
                    }
                }
            }

            MouseArea {
                y: dlgChart.plotArea.y
                x: dlgChart.plotArea.x
                width: dlgChart.plotArea.width
                height: dlgChart.plotArea.height
                property int lastAddedPoint: -1

                onPressed: function(mouse) {
                    if (mouse.button === Qt.LeftButton) {
                        let dlgPoint = mapToItem(dlgChart, Qt.point(mouse.x, mouse.y))
                        let modelPoint = dlgChart.mapToValue(dlgPoint, dlgChart.series(0))
                        lastAddedPoint = curve.addPoint(modelPoint)
                        curve.beginMovePoint(lastAddedPoint)
                    }
                }

                onReleased: function(mouse) {
                    if (mouse.button === Qt.LeftButton) {
                        curve.endMovePoint()
                    }
                }

                onPositionChanged: function(mouse) {
                    if (lastAddedPoint === curve.movingPointIndex) {
                        let dlgPoint = mapToItem(dlgChart, Qt.point(mouse.x, mouse.y))
                        let modelPoint = dlgChart.mapToValue(dlgPoint, dlgChart.series(0))
                        curve.movePoint(modelPoint);
                    }
                }

            }
        }
    }

    FocusScope {
        id: topFocus
        anchors.fill: parent

        SSoftValidatedField {
            id: nameField
            validator: IdentifierValidator {}
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            rightPadding: removeButton.width
            text: curve.name

            function set(value) {
                if (!text)
                    return "name cannot be empty"
                if (!curve.rename(text))
                    return "name already used"
                curve.pushChanges()
                return ""
            }

            Button {
                id: removeButton
                anchors.top: parent.top
                anchors.right: parent.right
                height: parent.height + parent.topPadding  - parent.bottomPadding
                width: height
                flat: true
                topInset: 0
                bottomInset: 0
                padding: 0
                icon.name: "window-close-symbolic"
                icon.color: topFocus.activeFocus || hovered ? Material.accent : Material.foreground
                onClicked: curve.remove()
            }
        }

        CurveChartView {
            curve: top.curve
            anchors.top: nameField.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left

            MouseArea {
                anchors.fill: parent
                onClicked: curveEditorDlg.open()
            }
        }
    }
}
