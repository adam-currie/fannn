import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

ApplicationWindow {
    id: window
    minimumHeight: 440
    minimumWidth: 440
    width: 700
    height: 700
    visible: true
    title: qsTr("Fannn Profile Editor")

    required property var builtInStyles
    required property var profilesModel
    required property var sensorsModel
    required property var curvesModel
    required property var governorsModel
    required property var controllersModel
    required property var profileModel

    Settings {
        id: settings
        property string style
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
            id: headerTitle
            anchors.centerIn: parent
            text: "Fannn"
            font.pixelSize: 18
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }
        ComboBox {
            //todo: size to content(without overlapping header title)
            anchors.right: headerTitle.left
            anchors.left: parent.left
            flat: true
            model: window.profilesModel
            textRole: "name"
            onActivated: model.loadProfile(currentValue)
        }
        ToolButton {
            icon.name: "view-more-symbolic"
            anchors.right: parent.right
            action: Action {
                onTriggered: optionsMenu.open()
            }

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

    property var settingsModel: [
        qsTr("Settings 1.1"),
        qsTr("Settings 1.2"),
        qsTr("Settings 1.3")
    ]

    Flickable {
        width: parent.width
        height: parent.height
        contentHeight: column.height
        ScrollBar.vertical: ScrollBar { }
        Column {
            id: column
            width: parent.width
            Row {
                width: parent.width
                height: childrenRect.height
                Label {
                    text: "update interval(ms): "
                }
                TextField {
                    text: window.profileModel.updateIntervalMs
                    onTextChanged:{
                        if(window.profileModel !== null) {
                            window.profileModel.updateIntervalMs = text
                        }
                    }
                }
            }

            Collapsible {
                width: parent.width
                title: "sensors"
                model: window.sensorsModel
                delegate: SensorDelegate {}
            }
        }
    }

    footer: ToolBar {
        Button {
            text: "save profile"
            flat: true
            anchors.right: parent.right
            enabled: window.profileModel && window.profileModel.unsavedChanges
            visible: enabled
            onClicked: window.profileModel.save()
        }
    }
}