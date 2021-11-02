import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

SpacedGridDelegate {
    required property var sensors

    required property int index
    required property var name
    required property var _alias
    required property var value

    id: top

    SSoftValidatedField {
        id: aliasOrNameText
        validator: IdentifierValidator {}
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left

        property var aliasOrName: (_alias)? _alias : top.name

        text: aliasOrName

        //overload
        function set(v) {
            if (!v)  //no error, we will reset when finished editing
                return ""

            if (v === name) {
                sensors.removeAlias(index)
                return ""
            }

            switch (sensors.setAlias(index, v)) {
                case ProfileModel.CollidesWithGovernor:
                    return "alias used by governor"
                case ProfileModel.CollidesWithSensorAlias:
                    return "alias used by another sensor"
                case ProfileModel.NoCollision:
                    return ""
                default:
                    console.error("whats the error?")
                    return "error"
            }
        }

        onEditingFinished: {
            if (!text) {
                sensors.removeAlias(index)
                text = aliasOrName
            }
        }
    }

    Text {
        id: nameText
        anchors.left: parent.left
        anchors.top: aliasOrNameText.bottom
        anchors.margins: 4
        anchors.leftMargin: aliasOrNameText.leftPadding
        text: top._alias? top.name : ""
        visible: text !== ""? true : false
        height: visible? contentHeight : 0
        wrapMode: Text.WrapAnywhere
        color: Material.foreground
    }

    Text {
        anchors.top: nameText.bottom
        anchors.left: parent.left
        anchors.margins: 4
        anchors.leftMargin: aliasOrNameText.leftPadding
        id: valueText
        color: Material.foreground
        text: "value: " + top.value
    }
}
