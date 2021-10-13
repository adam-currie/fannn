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

    Item {
        id: nameText
        height: childrenRect.height
        width:parent.width

        //todo: x button visible when text field has focus, remove alias

        TextField {
            id: field
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            text: name
            color: Material.foreground

            onEditingFinished: {
                if (text !== name) {
                    var result = profile.setAliasForSensor(name, text)
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
            anchors.left: parent.left
            anchors.top: field.bottom
            text: top._alias? top.name : ""
            visible: text != ""? true : false
            height: visible? contentHeight : 0
            wrapMode: Text.WrapAnywhere
            color: Material.foreground
        }
    }

    Text {
        anchors.margins: 5
        id: valueText
        anchors.top: nameText.bottom
        color: Material.foreground
        text: "value: " + top.value
    }
}
