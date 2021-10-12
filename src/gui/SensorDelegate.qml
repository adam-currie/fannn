import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

SpacedGridDelegate {
    required property var name
    required property var _alias
    required property var value

    id: top

    AliasText {
        id: nameText
        width: parent.width
        name: top.name
        _alias: top._alias
    }

    Text {
        anchors.margins: 5
        id: valueText
        anchors.top: nameText.bottom
        color: Material.foreground
        text: "value: " + top.value
    }
}
