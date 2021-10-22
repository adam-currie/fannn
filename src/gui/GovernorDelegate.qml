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

            property bool _blankNameText: false

            text: (_blankNameText)? "" : name

            onEditingFinished: {
                _blankNameText = false
            }

            onTextChanged: {
                var nextBlankNameText = !text
                //can't set actual value until after we set the text, or our text will get overwritten!
                if (!nextBlankNameText) {
                    var result = governors.rename(index, text)
                    //todo
                }
                _blankNameText = nextBlankNameText
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
