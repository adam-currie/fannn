import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

SpacedGridDelegate {
    required property var name
    required property var _alias
    required property var value
    required property var profile

    id: top

    STextField {
        id: aliasOrNameText
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottomMargin: 0
        text: (top._alias && top._alias !== "")? top._alias : top.name

        onTextChanged: {
            if (!profile)
                return
            if (text !== name) {
                var result = profile.addOrUpdateSensorAlias(name, text)
                switch (result) {
                    case ProfileModel.AliasCollidesWithSensorAlias: {
                        //todo: dialog
                        break;
                    }
                    case ProfileModel.AliasCollidesWithGovernor: {
                        //todo: dialog
                        break;
                    }
                }
            } else {
                profile.removeAliasForSensor(name)
            }
        }
    }

    Text {
        id: nameText
        anchors.left: parent.left
        anchors.top: aliasOrNameText.bottom
        anchors.margins: 4
        anchors.leftMargin: aliasOrNameText.padding
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
        anchors.leftMargin: aliasOrNameText.padding
        id: valueText
        color: Material.foreground
        text: "value: " + top.value
    }
}
