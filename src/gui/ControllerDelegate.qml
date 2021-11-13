import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Fannn

SpacedGridDelegate {
    required property int index
    required property var name
    required property var selectedGovernorIndex
    required property var controllers
    required property var governorChoices

    id: top

    //for some reason doing this in connections object didn't work?
    onGovernorChoicesChanged: {
        govNameBox.changingModel = true;
        govNameBox.model = top.governorChoices;

        if (selectedGovernorIndex)
            govNameBox.currentIndex = selectedGovernorIndex

        govNameBox.changingModel = false;
    }

    onSelectedGovernorIndexChanged: {
        govNameBox.currentIndex = selectedGovernorIndex
    }

    FocusScope {
        id: topFocus
        anchors.fill: parent

        STextField {
            id: nameField
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            text: top.name
            readOnly: true
            wrapMode: "WordWrap"
        }

        Label {
            id: govLabel
            anchors.left: govNameBox.left
            anchors.bottom: govNameBox.top
            verticalAlignment: Text.AlignVCenter
            text: "governor"
            anchors.bottomMargin: -3
            anchors.leftMargin: 0
            font.pointSize: 10
        }

        ComboBox {
            id: govNameBox
            anchors.left: parent.left
            anchors.top: nameField.bottom
            anchors.right: parent.right //todo: Math.min(size_to_content, size_to_parent)
            anchors.topMargin: 16
            anchors.margins: 6
            width: Math.min(implicitWidth, parent.width - (anchors.rightMargin + anchors.rightMargin))

            property bool changingModel: false

            onCurrentIndexChanged: {
                if (!changingModel) {
                    controllers.setGovernor(top.index, govNameBox.currentIndex)
                }
            }
        }
    }
}
