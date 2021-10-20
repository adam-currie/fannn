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
    required property var controllersModel

    Material.theme: Material.Dark
    Material.primary: Material.BlueGrey
    Material.accent: "#ff793b"

    Item {
        id: windowAreaItem
        anchors.fill: parent
    }

    SensorListModel {
        id: sensorsModel
        profile: profilesModel.currentProfile
    }

    GovernorListModel {
        id: governorsModel
        profile: profilesModel.currentProfile
        sensors: sensorsModel
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
        focusPolicy: Qt.ClickFocus

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
                parent: windowAreaItem
                profileModel: profilesModel.currentProfile
                onAccepted: addButton.addProfile()
                onDiscarded: addButton.addProfile()
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


        Pane {
            anchors.fill: parent
            focusPolicy: Qt.ClickFocus
        }

        Column {
            id: column
            width: parent.width
            Row {
                anchors.left: parent.left
                anchors.leftMargin: 10
                height: 50
                width: parent.width
                spacing: 5
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -2
                    text: "update interval(ms): "
                }
                STextField {
                    padding: 4
                    id: updateIntervalField
                    anchors.bottom: parent.bottom
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
            Collapsible {
                width: parent.width
                title: "Sensors"
                SpacedGridView {
                    width: parent.width
                    minCellWidth: 200
                    delegate: SensorDelegate {
                        profile: profilesModel.currentProfile
                    }
                    model: sensorsModel
                }
            }
            Collapsible {
                width: parent.width
                title: "Governors"

                SpacedGridView {
                    id: govGrid
                    width: parent.width
                    minCellWidth: 250
                    cellHeight: 200
                    delegate: GovernorDelegate {
                        governors: governorsModel
                    }
                    model: governorsModel
                }
                GridViewCellPredictor {
                    grid: govGrid
                    RoundButton {
                        y: parent.height/2 - height/2
                        x: y
                        width: 64
                        height: 64
                        Material.background: Material.accent
                        Material.elevation: 0
                        icon.name: "list-add-symbolic"
                        onClicked: governorsModel.add()
                    }
                }
            }
        }
    }

    footer: ToolBar {
        focusPolicy: Qt.ClickFocus
        Button {
            text: "save profile"
            flat: true
            anchors.right: parent.right
            anchors.margins: 5
            enabled: profilesModel.currentProfile ?
                profilesModel.currentProfile.unsavedChanges : false
            onClicked: profilesModel.currentProfile.save()
        }
    }
}

