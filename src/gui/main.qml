import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

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
    required property var curvesModel
    required property var governorsModel
    required property var controllersModel

    SensorListModel {
        id: sensorsModel
        profile: profilesModel.currentProfile
    }

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
        ComboBox {
            id: bongoBox
            property var bonusWidth: 60
            property var minWidth: 10
            popup.onAboutToHide: { minWidth = 10 }
            width: Math.max(minWidth, contentItem.contentWidth + bonusWidth)
            anchors.left: parent.left
            flat: true
            model: window.profilesModel
            textRole: "name"
            onActivated: model.loadProfile(currentValue)
            popup.onAboutToShow: {
                var widest = 0
                var originalCurrentIndex = currentIndex
                model.loadProfileNames()
                do {
                  widest = Math.max(widest, contentItem.contentWidth)
                  currentIndex = (currentIndex + 1) % count
                } while(currentIndex !== originalCurrentIndex)
                minWidth = widest + bonusWidth
            }
            Connections {
                target: bongoBox.model
                function onCurrentProfileChanged(p) {
                    bongoBox.currentIndex = bongoBox.model.indexOf(p.name)
                }
            }
        }
        RoundButton {
            icon.name: "list-add-symbolic"
            anchors {
                left: bongoBox.right
                leftMargin: -5
            }
            radius: 0
            flat: true
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
                    id: textField
                    text: profilesModel.currentProfile ?
                        profilesModel.currentProfile.updateIntervalMs : ""
                    onTextChanged: {
                        if(profilesModel.currentProfile) {
                            profilesModel.currentProfile.updateIntervalMs = text
                        }
                    }
                }
            }

            Collapsible {
                width: parent.width
                title: "sensors"
                model: sensorsModel
                delegate: SensorDelegate {}
            }
        }
    }

    footer: ToolBar {
        Button {
            text: "save profile"
            flat: true
            anchors.right: parent.right
            enabled: profilesModel.currentProfile ?
                profilesModel.currentProfile.unsavedChanges : false
            visible: enabled
            onClicked: profilesModel.currentProfile.save()
        }
    }
}

