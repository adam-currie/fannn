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

    property bool _blankAliasText: false

    id: top

    STextField {
        id: aliasOrNameText
        validator: IdentifierValidator {}
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottomMargin: 0
        text: (_blankAliasText)?
                  "" :
                  (_alias)? _alias : top.name

        onEditingFinished: {
            if (_blankAliasText) {
                sensors.removeAlias(index)
                _blankAliasText = false
            }
        }

        onTextChanged: {
            if (!sensors)
                return;

            var nextBlankAliasText = !text
            //can't set actual value until after we set the text, or our text will get overwritten!
            if (!nextBlankAliasText) {
                if (text === name) {
                    sensors.removeAlias(index)
                } else {
                    var result = sensors.setAlias(index, text)
                    switch (result) {
                        case ProfileModel.CollidesWithSensorAlias: {
                            //todo: dialog
                            break;
                        }
                        case ProfileModel.CollidesWithGovernor: {
                            //todo: dialog
                            break;
                        }
                    }
                }
            }
            _blankAliasText = nextBlankAliasText
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
