import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Fannn

SpacedGridDelegate {
    required property var curves

    required property int index
    required property var name

    id: top

    STextField {
        id: nameField
        validator: IdentifierValidator {}
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        bottomPadding: 0
        rightPadding: removeButton.width

        property bool _blankNameText: false

        text: (_blankNameText)? "" : name

        onEditingFinished: {
            _blankNameText = false
        }

        onTextChanged: {
            var nextBlankNameText = !text
            //can't set actual value until after we set the text, or our text will get overwritten!
            if (!nextBlankNameText) {
                var result = governors.rename(index, text)
                //todo
            }
            _blankNameText = nextBlankNameText
        }

        Button {
            id: removeButton
            anchors.top: parent.top
            anchors.right: parent.right
            height: parent.height + parent.topPadding
            width: height
            flat: true
            topInset: 0
            bottomInset: 0
            padding: 0
            icon.name: "window-close-symbolic"
            icon.color: topFocus.activeFocus || hovered ? Material.accent : Material.foreground
            onClicked: governors.remove(index)
        }
    }
}
