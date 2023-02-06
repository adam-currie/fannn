import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material

ApplicationWindow {
    id: window
    minimumHeight: 580
    minimumWidth: 440
    visibility: Window.Windowed
    width: 440
    height: 580
    visible: true
    title: qsTr("Permissions Requested - Fannn")

    //todo
    Material.theme: Material.Dark
    Material.primary: Material.BlueGrey
    Material.accent: "#ff793b"

    required property var pathsModel

    Settings {
        id: settings
    }

    Column {
        id: column
        width: parent.width
        padding: 16

        Label {
            text: "Write permission requested for:"
        }

        Rectangle {
            width: parent.width - (parent.leftPadding+parent.rightPadding)
            height: 80 //todo: max and min
            color: Material.background
            border.color: Color.blend(Material.background, Material.foreground, .3)

            //todo: if there are more then a couple files we should have a large box to show them, but not massive, its a huge number of files, show a scroll bar and force the user to scroll before accepting
            ListView {
                id: pathListView
                anchors.fill: parent
                model: pathsModel
                clip: true
                delegate: Label {
                    text: modelData
                }
                ScrollBar.vertical: ScrollBar { }
            }
        }

        TextInput {
            id: debug
            text: 'ff'
        }

        RoundButton {
            id: addButton
            text: 'ff'
        }

        Row {
            anchors.left: parent.left
            anchors.leftMargin: 10
            height: 50
            width: parent.width
            spacing: 5
            Label {
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -2
                text: "update interval(ms)"
            }
        }

    }

}
