import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

Item {
    property string name
    property string _alias

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
        visible: _alias && _alias != ""
        anchors.left: parent.left
        anchors.top: field.bottom
        text: _alias
        wrapMode: Text.WrapAnywhere
        color: Material.foreground
    }
}
