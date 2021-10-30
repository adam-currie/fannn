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
        id: badNameDlg
    }

    FocusScope {
        id: topFocus
        anchors.fill: parent

        STextField {
            id: nameField
            validator: IdentifierValidator {}
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            bottomPadding: 0
            rightPadding: removeButton.width

            property string _nameAtStartEditing: ""
            property int _nameCollision: ProfileModel.NoCollision

            text: curve.name

            onEditingFinished: {
                if (!text) {
                    badNameDlg.title = "name cannot be empty"
                    badNameDlg.open()
                    text = _nameAtStartEditing
                } else if (_nameCollision) {
                    badNameDlg.title = "name already used"
                    badNameDlg.open()
                    text = _nameAtStartEditing
                }

                _nameCollision = false
                _nameAtStartEditing = ""
            }

            onTextChanged: {
                if (!_nameAtStartEditing)
                    _nameAtStartEditing = curve.name
                if (text) {
                    _nameCollision = !curve.rename(text)
                    if (!_nameCollision) {
                        curve.pushChanges();
                    }
                }
            }

            Button {
                id: removeButton
                anchors.top: parent.top
                anchors.right: parent.right
                height: parent.height + parent.topPadding
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

        Dialog {
            id: curveEditorDlg
            parent: everythingItem
            width: Math.min(700, parent.width)
            height: Math.min(700, parent.height)
            x: (parent.width-width)/2
            y: (parent.height-height)/2
            modal: true
            standardButtons: Dialog.NoButton

            CurveChartView {
                id: dlgChart
                curve: top.curve
                anchors.fill: parent

                Repeater {
                    model: curve

                    Rectangle {
                        id: indicator
                        required property int index
                        required property var _point

                        property real chartPointX: (_point.x * dlgChart.plotArea.width / 100) + dlgChart.plotArea.x //todo: less magic
                        property real chartPointY: dlgChart.plotArea.height - (_point.y * dlgChart.plotArea.height / 100) + dlgChart.plotArea.y //todo: less magic

                        radius: 10
                        width: radius * 2
                        height: radius * 2
                        color: "orange"//todo
                        x: chartPointX - radius
                        y: chartPointY - radius

                        Item {
                            id: handle
                            parent: dlgChart
                            width: indicator.width
                            height: indicator.height

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

        CurveChartView {
            curve: top.curve
            anchors.top: nameField.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            margins.top: 10
            margins.left: 0
            margins.right: 0
            margins.bottom: 0

            MouseArea {
                anchors.fill: parent
                onClicked: curveEditorDlg.open()
            }
        }
    }
}
