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

    STextField {
        id: aliasOrNameText
        validator: IdentifierValidator {}
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottomMargin: 0

        property bool _isEditing: false
        property string _aliasAtStartOfEditing: ""
        property int _aliasCollision: ProfileModel.NoCollision

        property var aliasOrName: (_alias)? _alias : top.name

        Dialog {
            id: badAliasDlg
        }

        text: aliasOrName

        onEditingFinished: {
            var nameBad = false
            if (!text) {
                badAliasDlg.title = "alias cannot be empty"
                nameBad = true
            } else if (_aliasCollision === ProfileModel.CollidesWithGovernor) {
                badAliasDlg.title = "alias already used by governor name"
                nameBad = true
            } else if (_aliasCollision === ProfileModel.CollidesWithSensorAlias) {
                badAliasDlg.title = "alias already used by another sensor"
                nameBad = true
            }

            if (nameBad) {
                badAliasDlg.open()
                if (!_aliasAtStartOfEditing) {
                    sensors.removeAlias(index)
                    text = aliasOrName
                } else {
                    text = _aliasAtStartOfEditing
                }
            }

            _aliasCollision = ProfileModel.NoCollision
            _aliasAtStartOfEditing = ""
            _isEditing = false
        }

        onTextChanged: {
            if (!_isEditing) {
                _aliasAtStartOfEditing = (_alias)? _alias : null
                _isEditing = true
            }

            // null is fine here, just not empty
            if (text === null || text === name) {
                sensors.removeAlias(index)
            } else if (text.length > 0) {
                _aliasCollision = sensors.setAlias(index, text)
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
