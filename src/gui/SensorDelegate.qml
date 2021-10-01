import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

Rectangle{
    Text {
        anchors.margins: 5
        id: nameText
        text: "name: " + name
    }
    Text {
        anchors.margins: 5
        id: aliasText
        anchors.left: nameText.right
        text: "alias: " + _alias
    }
    Text {
        anchors.margins: 5
        id: valueText
        anchors.top: nameText.bottom
        text: "value: " + value
    }
}
