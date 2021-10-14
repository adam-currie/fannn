import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

SpacedGridDelegate {
    required property var name
    required property var governors

    id: top

    TextField {
        id: nameField
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: 5
        text: name
        color: Material.foreground

        onEditingFinished: {
            if (text !== name) {
                //todo
            }
        }
    }
}
