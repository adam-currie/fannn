import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Fannn Profile Editor")


    header: ToolBar {
        Label {
            anchors.fill: parent
            text: "Fannn"
            font.pixelSize: 18
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }
        RowLayout {
            anchors.fill: parent

            ComboBox {
                flat: true
                Layout.leftMargin: 8
                model: 3
                delegate: ItemDelegate {
                    text: "profile " + (index + 1)
                    width: listView.width
                }
            }

            ToolButton {
                icon.name: "view-more-symbolic"
                Layout.alignment: Qt.AlignRight

                Menu {
                    transformOrigin: Menu.TopRight

                    Action {
                        text: "Settings"
                    }
                    Action {
                        text: "About"
                    }
                }
            }
        }
    }

    property var settingsModel: [
        qsTr("Settings 1.1"),
        qsTr("Settings 1.2"),
        qsTr("Settings 1.3")
    ]

    Collapsible {
        width: parent.width
        title: "wow i'm collapsible"

            model: ListModel {
                ListElement {
                    name: "Bill Smith"
                    number: "555 3264"
                }
                ListElement {
                    name: "John Brown"
                    number: "555 8426"
                }
                ListElement {
                    name: "Sam Wise"
                    number: "555 0473"
                }
            }
            delegate: Text {
                text: name + ": " + number
            }
    }
}
