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

        SSoftValidatedField {
            id: nameField
            validator: IdentifierValidator {}
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            rightPadding: removeButton.width
            text: name

            //overload
            function set(v) {
                if (!text)
                    return "name cannot be empty"

                switch (governors.rename(index, text)) {
                    case ProfileModel.CollidesWithGovernor:
                        return "name used by another governor"
                    case ProfileModel.CollidesWithSensorAlias:
                        return "name used by a sensor alias"
                    case ProfileModel.NoCollision:
                        return ""
                    default:
                        console.error("whats the error?")
                        return "error"
                }
            }

            Button {
                id: removeButton
                anchors.top: parent.top
                anchors.right: parent.right
                height: parent.height + parent.topPadding - parent.bottomPadding
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
            anchors.topMargin: 0
            anchors.bottomMargin: -5

            TextArea {
                id: expTextArea
                leftPadding: nameField.leftPadding
                wrapMode: TextEdit.Wrap//todo: wrap on more charecters(example: sensor1+sensor2 should wrap at '+')
                onTextChanged: governors.setExpression(index, text)
                selectByMouse: true
                text: expression
                placeholderText: "e.g. curve1(governor1+sensor1)*2"
                ErrorHighlighter {
                    id: errorHighlighter
                    document: expTextArea.textDocument
                }

                Component.onCompleted: errorHighlighter.formatErrors(errors)
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
            anchors.topMargin: 0
            topPadding: -3
            width: parent.width
            height: 30
            contentWidth: availableWidth
            contentHeight: warning.height
            Warning {
                id: warning
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                leftPadding: nameField.leftPadding
                wrapMode: TextEdit.Wrap
                verticalAlignment: Text.AlignVCenter
                lineHeight: .666
                text: errorStr//debug errorStr
            }
        }
    }
}
