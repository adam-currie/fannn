import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

SpacedGridDelegate {
    required property var name
    required property var governors

    id: top

    STextField {
        id: nameField
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        bottomPadding: 0
        rightPadding: removeButton.width
        text: name

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
            icon.color: activeFocus || hovered ? Material.accent : Material.foreground
            onClicked: governors.remove(name)
        }
    }

}
