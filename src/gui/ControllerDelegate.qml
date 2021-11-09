import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Fannn

SpacedGridDelegate {
    required property int index
    required property var name
    required property var governorName
    required property var controllers
    required property var governors
    property var everythingItem: this

    id: top

    FocusScope {
        id: topFocus
        anchors.fill: parent

        STextField {
            id: nameField
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            text: top.name
            readOnly: true
            wrapMode: "WordWrap"
        }

        Label {
            id: govLabel
            anchors.left: govNameBox.left
            anchors.bottom: govNameBox.top
            verticalAlignment: Text.AlignVCenter
            text: "governor"
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            font.pointSize: 9
            color: Material.hintTextColor
        }

        NoneOptionComboBox {
            id: govNameBox
            everythingItem: top.everythingItem
            textRole: "name"
            anchors.left: parent.left
            anchors.top: nameField.bottom
            anchors.right: parent.right //todo: Math.min(size_to_content, size_to_parent)
            anchors.topMargin: 18
            anchors.margins: 6
            width: Math.min(implicitWidth, parent.width - (anchors.rightMargin + anchors.rightMargin))
            model: governors

            onCurrentTextChanged: {
                if (govNameBox.currentIndex >= 0) {
                    controllers.setGovernor(index, govNameBox.currentText);
                } else {
                    controllers.removeGovernor(index);
                }
            }
        }
    }
}
