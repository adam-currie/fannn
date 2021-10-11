import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

Rectangle{
    required property var name
    required property var _alias
    required property var value

    color: Material.foreground

    Text {
        anchors.margins: 5
        id: nameText
        text: "name: " + name
    }
    Text {
        anchors.margins: 5
        id: aliasText
        anchors.top: nameText.bottom
        text: "alias: " + _alias
    }
    Text {
        anchors.margins: 5
        id: valueText
        anchors.top: aliasText.bottom
        text: "value: " + parent.value
    }
}
