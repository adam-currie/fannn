import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material
import Fannn

ApplicationWindow {
    id: window
    minimumHeight: 440
    minimumWidth: 440
    width: 700
    height: 700
    visible: true
    title: qsTr("Fannn Profile Editor")

    required property var profilesModel
    required property var curvesModel
    required property var governorsModel
    required property var controllersModel

    Material.theme: Material.Dark
    Material.primary: Material.BlueGrey
    Material.accent: Material.Pink

    SensorListModel {
        id: sensorsModel
        profile: profilesModel.currentProfile
    }

    Settings {
        id: settings
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
            settingsDialog.close()
        }
        onRejected: {
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 8

            RowLayout {
                spacing: 8
            }

        }
    }

    header: ToolBar {
        ProfilesComboBox {
            id: bongoBox
            model: window.profilesModel
            anchors {
                left: parent.left
                leftMargin: 5
            }
        }
        RoundButton {
            id: addButton

            function addProfile(){
                profilesModel.createAndSwitchTo()
                //todo: focus ComboBox textfield for editing new name
            }

            UnsavedChangesDialog {
                id: confirmationDialog
                profileModel: profilesModel.currentProfile
                Connections {
                    target: confirmationDialog
                    function onAccepted() { addButton.addProfile() }
                    function onDiscarded() { addButton.addProfile() }
                    //onRejected do nothing
                }
            }

            icon.name: "list-add-symbolic"
            anchors {
                left: bongoBox.right
                leftMargin: -6
            }
            radius: 0
            flat: true
            onClicked: (profilesModel.currentProfile && profilesModel.currentProfile.unsavedChanges) ?
                           confirmationDialog.open() :
                           addProfile()
        }

        ToolButton {
            icon.name: "view-more-symbolic"
            anchors.right: parent.right
            onClicked: optionsMenu.open();

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
                    id: updateIntervalField
                    validator: IntValidator {bottom: 1}
                    text: profilesModel.currentProfile ?
                        profilesModel.currentProfile.updateIntervalMs : ""
                    onTextChanged: {
                        if(profilesModel.currentProfile) {
                            profilesModel.currentProfile.updateIntervalMs = text
                        }
                    }
                }
            }
            SensorsCollapsible {
                model: sensorsModel
                width: parent.width
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

