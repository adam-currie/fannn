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

            ChartView {
                anchors.fill:parent
                antialiasing: true
                backgroundColor: "transparent"
                legend.visible: false
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


            ValueAxis {
                id: axisX
                labelsColor: "white"//todo: not sure why Material.foreground doesn't work
                min: 0
                max: 100
                tickCount: 5
            }

            ValueAxis {
                id: axisY
                labelsColor: "white"//todo: not sure why Material.foreground doesn't work
                min: 0
                max: 100
                tickCount: 5
            }

            LineSeries {
                name: "LineSeries"
                axisX: axisX
                axisY: axisY
                XYPoint { x: 0; y: 0 }
                XYPoint { x: 1.1; y: 2.1 }
                XYPoint { x: 1.9; y: 3.3 }
                XYPoint { x: 2.1; y: 2.1 }
                XYPoint { x: 2.9; y: 4.9 }
                XYPoint { x: 3.4; y: 3.0 }
                XYPoint { x: 4.1; y: 3.3 }
            }
        }
    }
}
