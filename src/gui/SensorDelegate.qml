import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

SpacedGridDelegate {
    required property var name
    required property var _alias
    required property var value

    id: top

    Text {
        anchors.margins: 5
        id: nameText
        wrapMode: Text.WrapAnywhere
        width: top.width
        text: name
    }
    Text {
        anchors.margins: 5
        id: aliasText
        anchors.top: nameText.bottom
        text: "alias: " + top._alias
    }
    Text {
        anchors.margins: 5
        id: valueText
        anchors.top: aliasText.bottom
        text: "value: " + top.value
    }
}
