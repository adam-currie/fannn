import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import fannn 1.0

ApplicationWindow {
    id: window
    minimumHeight: 440
    minimumWidth: 440
    width: 700
    height: 700
    visible: true
    title: qsTr("Fannn Profile Editor")

    required property var builtInStyles

    Settings {
        id: settings
        property string style
    }

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        onTriggered: optionsMenu.open()
    }

    Dialog {
        id: settingsDialog
        x: Math.round((parent.width - width) / 2)
        y: Math.round(parent.height / 6)
        implicitWidth: 300
        width: Math.round(Math.min(parent.width, implicitWidth))
        modal: true
        focus: true
        title: "Settings"

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.style = styleBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            styleBox.currentIndex = styleBox.styleIndex
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 8

            RowLayout {
                spacing: 8

                Label {
                    text: "Style:"
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: window.builtInStyles
                    Component.onCompleted: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex
                    }
                    Layout.fillWidth: true
                }
            }

            Warning {
                text: "Restart required"
                opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignRight
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

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
                model: ProfilesModel
            }

            ToolButton {
                icon.name: "view-more-symbolic"
                Layout.alignment: Qt.AlignRight
                action: optionsMenuAction

                Menu {
                    id: optionsMenu
                    transformOrigin: Menu.TopRight

                    Action {
                        text: "Settings"
                        onTriggered: settingsDialog.open()
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

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent
        }
    }
}
