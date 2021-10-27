import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Fannn

SpacedGridDelegate {
    required property var name
    required property var expression
    required property var governors
    required property int index
    required property var errors
    required property var errorStr

    id: top

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

            text: name

            Dialog {
                id: badNameDlg
            }

            onEditingFinished: {
                var nameBad = false

                if (!text) {
                    badNameDlg.title = "name cannot be empty"
                    nameBad = true
                } else if (_nameCollision === ProfileModel.CollidesWithGovernor) {
                    badNameDlg.title = "name already used by another governor"
                    nameBad = true
                } else if (_nameCollision === ProfileModel.CollidesWithSensorAlias) {
                    badNameDlg.title = "name already used by a sensor alias"
                    nameBad = true
                }

                _nameCollision = ProfileModel.NoCollision
                if (nameBad) {
                    badNameDlg.open()
                    text = _nameAtStartEditing
                }
                _nameAtStartEditing = ""
            }

            onTextChanged: {
                if (!_nameAtStartEditing)
                    _nameAtStartEditing = name
                if (text)
                    _nameCollision = governors.rename(index, text)
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
                onClicked: governors.remove(index)
            }
        }

        ScrollView {
            anchors.top: nameField.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: errorBox.top
            anchors.topMargin: 10
            anchors.bottomMargin: -5
            TextArea {
                id: expTextArea
                padding: nameField.padding
                wrapMode: TextEdit.Wrap//todo: wrap on more charecters(example: sensor1+sensor2 should wrap at '+')
                onTextChanged: governors.setExpression(index, text)
                selectByMouse: true
                text: expression
                ErrorHighlighter {
                    id: errorHighlighter
                    document: expTextArea.textDocument
                }
                Connections {
                    target: top
                    function onErrorsChanged() {
                        errorHighlighter.formatErrors(errors)
                    }
                }
            }
        }

        ScrollView {
            id: errorBox
            anchors.bottom: parent.bottom
            anchors.margins: nameField.padding
            anchors.topMargin: 0
            width: parent.width
            height: 26
            contentWidth: availableWidth
            Warning {
                anchors.left: parent.left
                anchors.right: parent.right
                wrapMode: TextEdit.Wrap
                verticalAlignment: Text.AlignVCenter
                lineHeight: .666
                text: errorStr
            }
        }
    }
}
