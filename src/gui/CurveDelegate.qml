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
        closePolicy: curve.needsPush ? Dialog.NoAutoClose : (Dialog.CloseOnPressOutside | Dialog.CloseOnEscape)

        onAccepted: curve.pushChanges()
        onRejected: curve.discardChanges()

        //need to use our own buttons because DialogButtonBox doesnt work well with dynamic buttons
        Row {
            id: bottomButtons
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 6
            spacing: 5
            Button {
                text: "Close"
                flat: true
                visible: !curve.needsPush
                onClicked: curveEditorDlg.close()
            }
            Button {
                text: "Cancel"
                flat: true
                visible: curve.needsPush
                onClicked: curveEditorDlg.reject()
            }
            Button {
                text: "Accept"
                flat: true
                visible: curve.needsPush
                onClicked: curveEditorDlg.accept()
            }
        }

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
                    if (!value) {
                        return "name cannot be empty"
                    } else if (value === curve.name) {
                        return ""
                    } else if (curve.rename(value)) {
                        return ""
                    } else {
                        return "name already used"
                    }
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
                text: "x:" + curve.needsPush
                height: parent.height
                leftPadding: 14
                horizontalAlignment: TextInput.AlignRight
                verticalAlignment: TextInput.AlignVCenter
            }
            STextField {
                text: curve.minX
                onEditingFinished: curve.minX = text
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
                text: curve.maxX
                onEditingFinished: curve.maxX = text
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
                text: curve.minY
                onEditingFinished: curve.minY = text
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
                text: curve.maxY
                onEditingFinished: curve.maxY = text
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
            anchors.bottom: bottomButtons.top
            anchors.right: parent.right
            anchors.left: parent.left

            Repeater {
                model: curve

                Rectangle {
                    id: indicator
                    required property int index
                    required property var _point

                    property real chartPointX: ((_point.x-curve.minX) * dlgChart.plotArea.width / (curve.maxX-curve.minX)) + dlgChart.plotArea.x
                    property real chartPointY: dlgChart.plotArea.height - ((_point.y-curve.minY) * dlgChart.plotArea.height / (curve.maxY-curve.minY)) + dlgChart.plotArea.y

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
                if (!value) {
                    return "name cannot be empty"
                } else if (value === curve.name) {
                    return ""
                } else if (curve.rename(value)) {
                    curve.pushChanges()
                    return ""
                } else {
                    return "name already used"
                }
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
            ticks: 5

            MouseArea {
                anchors.fill: parent
                onClicked: curveEditorDlg.open()
            }
        }
    }
}
