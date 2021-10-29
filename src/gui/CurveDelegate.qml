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

    ValueAxis {
        id: axisX
        labelsColor: "white"//todo: not sure why Material.foreground doesn't work
        min: curve.minX
        max: curve.maxX
        tickCount: 5
    }

    ValueAxis {
        id: axisY
        labelsColor: "white"//todo: not sure why Material.foreground doesn't work
        min: curve.minY
        max: curve.maxY
        tickCount: 5
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

            ChartView {
                id: dlgChart
                anchors.fill: parent
                antialiasing: true
                backgroundColor: "transparent"
                legend.visible: false

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
                                preventStealing: true

                                drag.onActiveChanged: {
                                    if (drag.active)
                                        curve.beginMovePoint(index)
                                    else
                                        curve.endMovePoint()
                                }

                                function move() {
                                    let point = dlgChart.mapToValue(Qt.point(handle.x + width / 2, handle.y + height / 2), dlgSeries)
                                    curve.movePoint(point)
                                }

                                onMouseXChanged: { if (drag.active) move() }
                                onMouseYChanged: { if (drag.active) move() }
                            }
                        }
                    }
                }

                MouseArea {
                    y: dlgChart.plotArea.y
                    x: dlgChart.plotArea.x
                    width: dlgChart.plotArea.width
                    height: dlgChart.plotArea.height

                    onClicked: function(mouse) {
                        let dlgPoint = mapToItem(dlgChart, Qt.point(mouse.x, mouse.y))
                        let modelPoint = dlgChart.mapToValue(dlgPoint, dlgSeries)
                        console.log("onClicked: " + modelPoint.x + ", " + modelPoint.y);//debug
                        curve.addPoint(modelPoint)
                    }
                }

                ValueAxis {
                    id: dlgAxisX
                    labelsColor: "white"//todo: not sure why Material.foreground doesn't work
                    min: curve.minX
                    max: curve.maxX
                    tickCount: 5
                }

                ValueAxis {
                    id: dlgAxisY
                    labelsColor: "white"//todo: not sure why Material.foreground doesn't work
                    min: curve.minY
                    max: curve.maxY
                    tickCount: 5
                }

                LineSeries {
                    id: dlgSeries
                    axisX: dlgAxisX
                    axisY: dlgAxisY
                    //todo: color: Material.accent doesnt work, all of the material colours are off here for some reason, maybe we can make a conversion function
                }

                VXYModelMapper {
                    xColumn: 0
                    yColumn: 1
                    series: dlgSeries
                    model: curve
                }
            }
        }

        ChartView {
            anchors.top: nameField.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            antialiasing: true
            backgroundColor: "transparent"
            margins.top: 10
            margins.left: 0
            margins.right: 0
            margins.bottom: 0
            legend.visible: false

            MouseArea {
                anchors.fill: parent
                onClicked: curveEditorDlg.open()
            }

            LineSeries {
                id: series
                axisX: axisX
                axisY: axisY
            }

            VXYModelMapper {
                xColumn: 0
                yColumn: 1
                series: series
                model: curve
            }
        }
    }
}
