import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

Item {
    property var name
    property var _alias

    id: top
    height: childrenRect.height

    TextField {
        id: field
        anchors.top: top.top
        anchors.right: top.right
        anchors.left: top.left
        text: name
        color: Material.foreground

        onEditingFinished: {
            _alias = text
        }
    }

    Text {
        anchors.left: parent.left
        anchors.top: field.bottom
        text: top._alias? top._alias : ""
        visible: text != ""? true : false
        height: visible? contentHeight : 0
        wrapMode: Text.WrapAnywhere
        color: Material.foreground
    }
}
